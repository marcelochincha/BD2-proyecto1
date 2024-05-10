#include "isam.hpp"

//
// PUBLIC
//

// Constructor
Isam::Isam(std::string _filename) : File_manager(_filename) {
    if (!fileExists(this->dataName())) this->init_file();

    // Add index
    std::vector<T> keys = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    this->DEBUG_addIndexData(keys);
};

// Destructor
Isam::~Isam(){};

std::vector<Register> Isam::search(T key) {
    // Iniciar busqueda
    long nextPage = this->getDataPage(key);

    // Buscar en el data page seleccionado
    std::fstream f;
    std::vector<Register> found;
    f.open(this->dataName(), std::ios::binary | std::ios::in | std::ios::out);
    this->searchInPage(key, nextPage, found, f);
    f.close();
    return found;
};

std::vector<Register> Isam::range_search(T begin_key, T end_key) {
    return std::vector<Register>();
};

bool Isam::add(Register data) {
    // Iniciar busqueda
    long nextPage = getDataPage(data.id);
    //std::cout << "Next page: " << nextPage << std::endl;

    // Insertar en el data page seleccionado
    std::fstream f;
    f.open(this->dataName(), std::ios::binary | std::ios::in | std::ios::out);
    bool result = addToPage(data, nextPage, f);
    f.close();
    return result;
};

bool Isam::remove(T key) {
    return false;
};

// DEBUG
// Crea un page para cada indice
// Keys deberia estar ordenado y tener la misma distribucion
void Isam::DEBUG_addIndexData(std::vector<T> keys) {
    if (keys.size() > MI) {
        throw std::invalid_argument("Too many keys, reduce the number of keys to add.");
        return;
    }

    if (MAX_DEPTH != 1) {
        throw std::runtime_error("This value should be 1.");
        return;
    }

    std::fstream indxFile(idxName(0), std::ios::binary | std::ios::in | std::ios::out);
    std::fstream dataFile(this->dataName(), std::ios::binary | std::ios::in | std::ios::out);

    dataFile.seekp(0, std::ios::beg);
    indxFile.seekp(0, std::ios::beg);
    IndexPage indexBuffer{1};

    // Write default
    DataPage dataBuffer{0, -1};
    indexBuffer.pages[0] = dataFile.tellp();
    dataFile.write((char *)&dataBuffer, sizeof(DataPage));
    for (size_t i = 0; i < keys.size(); i++) {
        indexBuffer.keys[i] = keys[i];
        indexBuffer.pages[i + 1] = dataFile.tellp();
        // RE USE DATA PAGE
        dataFile.write((char *)&dataBuffer, sizeof(DataPage));
        indexBuffer.n++;
    }

    indxFile.write((char *)&indexBuffer, sizeof(IndexPage));
    indxFile.close();
    dataFile.close();
};

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
    f.seekg(pagePosition , std::ios::beg);
    f.read((char *)&toReturn, sizeof(PageType));
    return toReturn;
};

long Isam::returnPage(IndexPage &page, const T key) {
    long toReturn = page.pages[0];
    for (int i = 0; i < page.n; i++) {
        if (keyCmp(key,page.keys[i]) > 0)
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
    //std::cout << "Page pos: " << pagePos << std::endl;
    DataPage data = this->loadPage<DataPage>(pagePos, f);

    // Verificar si la página está llena
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
    bool founded = false;
    bool checkNext = false;
    for (int i = 0; i < data.n; i++) {
        if (keyCmp(key, data.records[i].id) == 0) {
            buffer.push_back(data.records[i]);
            founded = true;
        } else if (founded) {
            checkNext = true;
            break;
        }
    }
    if (checkNext) this->searchInPage(key, data.nextPage, buffer, f);
    return founded;
}