#include "Isam.hpp"

#include <cmath>

#include "consumer_id_data.hpp"

//
// PUBLIC
//

// Constructor
Isam::Isam(std::string _filename) : File_manager(_filename) {
    if (!fileExists(this->dataName())) {
        // Add index
        this->init_file();
        this->DEBUG_addIndexData(consumer_id);
    } else
        this->loaded = true;

    // this->showTree();
};

void Isam::showTree() {
    std::fstream f;
    f.open(this->idxName(1), std::ios::binary | std::ios::in);
    for (int i = 0; i < MI * MI; i++) {
        IndexPage currentIndexPage = this->loadPage<IndexPage>(i * sizeof(IndexPage), f);
        std::cout << "Index PAGE:  " << i << " loc: " << (i * sizeof(IndexPage)) << std::endl;
        for (int i = 0; i < currentIndexPage.n; i++) {
            std::cout << currentIndexPage.pages[i] << " ";
        }
        std::cout << std::endl;
    }
}

// Destructor
Isam::~Isam(){};

std::vector<Register> Isam::search(T key) {
    // Iniciar busqueda
    long nextPage = this->getDataPage(key);

    // Buscar en el data page seleccionado
    std::fstream f;
    std::vector<Register> found;
    f.open(this->dataName(), std::ios::binary | std::ios::in | std::ios::out);
    bool res = this->searchInPage(key, nextPage, found, f);
    if (!res) {
        std::cout << "Not found" << std::endl;
    }
    f.close();
    return found;
};

std::vector<Register> Isam::range_search(T begin_key, T end_key) {
    std::cout << "Range search" << std::endl;
    std::vector<Register> buffer;
    std::fstream f;
    this->recursiveRangeSearch(begin_key, end_key, 0, 0, buffer, f);
    return buffer;
};

bool Isam::add(Register data) {
    // std::cout << "GOOD" << std::endl;
    //  Iniciar busqueda
    long nextPage = getDataPage(data.CustomerID);
    // std::cout << "Next page: " << nextPage << std::endl;

    // Insertar en el data page seleccionado
    std::fstream f;
    f.open(this->dataName(), std::ios::binary | std::ios::in | std::ios::out);
    bool result = addToPage(data, nextPage, f);
    f.close();
    return result;
};

bool Isam::remove(T key) {
    // Iniciar busqueda
    long nextPage = getDataPage(key);

    // Buscar en el data page seleccionado
    std::fstream f;
    std::vector<Register> found;
    f.open(this->dataName(), std::ios::binary | std::ios::in | std::ios::out);
    bool result = this->removeInPage(key, nextPage, f);
    f.close();

    return result;
};

// DEBUG
// Crea un page para cada indice
// Keys deberia estar ordenado y tener la misma distribucion
void Isam::DEBUG_addIndexData(const int keys[consumer_id_data_size]) {
    if (MAX_DEPTH < log(consumer_id_data_size) / log(MI)) {
        throw std::invalid_argument("Too many keys, reduce the number of keys to add." + std::to_string(MAX_DEPTH) +
                                    " < " + std::to_string(log(consumer_id_data_size) / log(MI)));
        return;
    }

    int n = consumer_id_data_size;

    std::cout << "Creating index with MI: " << MI << "& MD: " << MD << std::endl;
    // Create data
    std::fstream dataFile(this->dataName(), std::ios::binary | std::ios::in | std::ios::out);
    dataFile.seekp(0, std::ios::beg);

    for (int depth = 0; depth < MAX_DEPTH; depth++) {
        std::fstream indxFile;
        indxFile.open(this->idxName(depth), std::ios::binary | std::ios::in | std::ios::out);

        indxFile.seekp(0, std::ios::beg);

        int p_size = pow(MI, depth);
        int step_size = n / p_size;

        long pos = 0;
        for (size_t i = 0; i < p_size; i++) {
            int left = i * step_size;
            int right = (i + 1) * step_size;
            int inner_step = (right - left) / MI;

            if (inner_step == 0) inner_step = 1;

            // Crear index page
            IndexPage indexBuffer{0};
            // Rellenar
            indexBuffer.pages[0] = pos * sizeof(IndexPage);
            pos++;
            for (size_t j = 0; j < MI; j += 1) {
                if (j > MI) throw std::runtime_error("Index out of bounds");

                int next = left + j * inner_step;
                if (next >= n || next > consumer_id_data_size) break;
                indexBuffer.keys[j] = keys[next];
                indexBuffer.pages[j + 1] = pos * sizeof(IndexPage);
                pos++;
                indexBuffer.n++;

                // Crear data page
                DataPage dataBuffer{0, -1};
                dataFile.write((char *)&dataBuffer, sizeof(DataPage));
            }
            indxFile.write((char *)&indexBuffer, sizeof(IndexPage));
        }
        indxFile.close();
    }
    dataFile.close();
}

//
// PROTECTED
//

// Iniciar archivos
void Isam::init_file() {
    // Crear archivos de índice
    std::fstream f;
    for (int i = 0; i < MAX_DEPTH; i++) {
        f.open(this->idxName(i), std::ios::binary | std::ios::in | std::ios::out | std::ios::app);
        f.close();
    }

    // Crear archivo de datos
    f.open(this->dataName(), std::ios::binary | std::ios::in | std::ios::out | std::ios::app);
};

//
// PRIVATE
//

// HELPERS
template <typename PageType>
PageType Isam::loadPage(long pagePosition, std::fstream &f) {
    PageType toReturn{};
    f.seekg(pagePosition, std::ios::beg);
    f.read((char *)&toReturn, sizeof(PageType));
    return toReturn;
};

long Isam::returnPage(IndexPage &page, const T key) {
    long toReturn = page.pages[0];
    for (int i = 0; i < page.n; i++) {
        if (keyCmp(key, page.keys[i]) > 0)
            toReturn = page.pages[i + 1];
        else
            break;
    }
    return toReturn;
};

long Isam::getDataPage(const T key) {
    std::fstream f;
    int currentLevel = 0;
    long nextPage = 0;

    // Buscar la pagina en los indexpages
    while (currentLevel < MAX_DEPTH) {
        f.open(idxName(currentLevel), std::ios::binary | std::ios::in);
        IndexPage currentIndexPage = this->loadPage<IndexPage>(nextPage, f);
        f.close();
        if (currentIndexPage.n > MI) 
            throw std::runtime_error("Index page is OVERFITTED?");
        nextPage = this->returnPage(currentIndexPage, key);
        currentLevel++;
    }

    return nextPage;
}

bool Isam::fileExists(const std::string &name) {
    std::ifstream f(name.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    return f.good();
};

std::string Isam::idxName(int i) {
    return this->filename + std::to_string(i) + IDX_EXT;
};

std::string Isam::dataName() {
    return this->filename + DAT_EXT;
};

// OPERACIONES
bool Isam::addToPage(const Register &record, long pagePos, std::fstream &f) {
    // Cargar la página
    // std::cout << "Page pos: " << pagePos << std::endl;
    DataPage data = this->loadPage<DataPage>(pagePos, f);

    if (MD == 0) {
        throw std::runtime_error("MD is 0");
    }

    // Verificar si la página está llena
    // std::cout << "Data n: " << data.n << std::endl;
    if (data.n >= MD) {
        // Crear nueva pagina si lo esta
        if (data.nextPage == -1) {
            DataPage newData{0, -1};
            f.seekp(0, std::ios::end);
            long newPagePos = f.tellp();
            f.write((char *)&newData, sizeof(DataPage));

            // Actualizar data
            data.nextPage = newPagePos;
            f.seekp(pagePos, std::ios::beg);
            f.write((char *)&data, sizeof(DataPage));
        }
        // Ir a la pagina que se encuentra en nextPage
        return this->addToPage(record, data.nextPage, f);
    }

    // Añadir el registro a la pagina
    data.records[data.n] = record;
    data.n++;
    f.seekp(pagePos, std::ios::beg);
    f.write((char *)&data, sizeof(DataPage));
    return true;
}

bool Isam::searchInPage(const T key, long pagePos, std::vector<Register> &buffer, std::fstream &f) {
    DataPage data = this->loadPage<DataPage>(pagePos, f);

    for (int i = 0; i < data.n; i++) {
        if (keyCmp(key, data.records[i].CustomerID) == 0) 
            buffer.push_back(data.records[i]);
    }

    //Verificar si hay una siguiente pagina
    if (data.nextPage != -1) 
        return this->searchInPage(key, data.nextPage, buffer, f);
    
    return buffer.size() > 0;

}

bool Isam::recursiveRangeSearch(const T begin_key, const T end_key, int depth, long posPage,
                                std::vector<Register> &buffer, std::fstream &f) {
    // Buscar la pagina en los indexpages
    if (depth < MAX_DEPTH) {
        f.open(idxName(depth), std::ios::binary | std::ios::in);
        IndexPage currentIndexPage = this->loadPage<IndexPage>(posPage, f);
        f.close();
        for (int i = 0; i < currentIndexPage.n; i++) {
            if (keyCmp(begin_key, currentIndexPage.keys[i]) <= 0 && keyCmp(end_key, currentIndexPage.keys[i]) >= 0) {
                recursiveRangeSearch(begin_key, end_key, depth + 1, currentIndexPage.pages[i], buffer, f);
            }
        }
        depth++;
    }

    return false;
}

bool Isam::rangeSearchInPage(const T begin_key, const T end_key, long pagePos, std::vector<Register> &buffer,
                             std::fstream &f) {
    DataPage data = this->loadPage<DataPage>(pagePos, f);
    bool founded = false;
    bool checkNext = false;
    for (int i = 0; i < data.n; i++) {
        if (keyCmp(begin_key, data.records[i].CustomerID) <= 0 && keyCmp(end_key, data.records[i].CustomerID) >= 0) {
            buffer.push_back(data.records[i]);
            founded = true;
        } else if (founded) {
            checkNext = true;
            break;
        }
    }
    if (checkNext) this->rangeSearchInPage(begin_key, end_key, data.nextPage, buffer, f);
    return founded;
}

bool Isam::removeInPage(const T key, long pagePos, std::fstream &f) {
    DataPage data = this->loadPage<DataPage>(pagePos, f);
    bool founded = false;
    bool checkNext = false;
    for (int i = 0; i < data.n; i++) {
        if (keyCmp(key, data.records[i].CustomerID) == 0) {
            founded = true;
            // Eliminar el registro (remplazar con el ultimo registro)
            data.records[i] = data.records[data.n - 1];
            data.n--;
            f.seekp(pagePos, std::ios::beg);
            f.write((char *)&data, sizeof(DataPage));
        } else if (founded) {
            checkNext = true;
            break;
        }
    }
    if (checkNext) this->removeInPage(key, data.nextPage, f);
    return founded;
}
