#pragma once

#include <fstream>
#include <string>
#include <vector>

#define MAX_DEPTH 1
#define PAGE_SIZE 4096

// MD y MI son el número de registros que caben en una página de datos e índice.
const int MI = (TAM_PAGE - sizeof(int) - sizeof(long)) / ((sizeof(int) + sizeof(long)));
const int MD = (TAM_PAGE - sizeof(int) - sizeof(long)) / (sizeof(Record));

// INDICE
struct IndexPage {
    int n;
    int keys[MI];
    long pages[MI + 1];
};

// DATOS
struct DataPage {
    int n;
    long nextPage;  // -1 si es el ultimo registro, en bytes desde el inicio del archivo
    Register records[MD];
};

// Clase para el manejo de archivos con ISAM
class Isam : public File_manager {
   protected:
    // Iniciar archivos
    void init_file() {
        // Crear archivos de índice
        std::fstream f;
        for (int i = 0; i < MAX_DEPTH; i++) {
            f.open(idxName(i), std::ios::binary | std::ios::in | std::ios::out);
            IndexPage indexPage{0};
            f.write((char *)&indexPage, sizeof(IndexPage));
            f.close();
        }

        // Crear archivo de datos
        f.open("data.dat", std::ios::binary | std::ios::in | std::ios::out);
    };

   public:
    Isam(std::string _filename) : File_manager(_filename) {
        if (!fileExists(_filename + EXT_STR)) this->init_file();
    };
    ~Isam(){};

    std::vector<Register> search(T key) {
        // Iniciar busqueda
        std::fstream f;
        long nextPage = this->getDataPage(f, data.id);

        // Buscar en el data page seleccionado
        std::vector<Record> found;
        f.open(this->filename + EXT_STR, std::ios::binary | std::ios::in | std::ios::out);
        this->searchInPage(data.id, nextPage, found, f);
        f.close();
        return found;
    };

    std::vector<Register> range_search(T begin_key, T end_key){};

    bool add(Register data) {
        // Iniciar busqueda
        std::fstream f;
        long nextPage = getDataPage(f, data.id);

        // Insertar en el data page seleccionado
        f.open(this->filename + EXT_STR, std::ios::binary | std::ios::in | std::ios::out);
        bool result = addToPage(record, nextPage, f);
        f.close();
        return result;
    };

    bool remove(T key) { return false; };

    // DEBUG
    // Crea un page para cada indice
    // Keys deberia estar ordenado y tener la misma distribucion
    void DEBUG_addIndexData(std::vector<T> keys) {
        if (keys.size() > MI) {
            throw std::invalid_argument("Too many keys, reduce the number of keys to add.");
            return;
        }

        if (MAX_DEPTH != 1) {
            throw std::bad_exception("This value should be 1.");
            return;
        }

        std::fstream indxFile(idxName(0), std::ios::binary | std::ios::in | std::ios::out);
        std::fstream dataFile(this->filename + EXT_STR, std::ios::binary | std::ios::in | std::ios::out);

        data.seekp(0, std::ios::beg);
        indxFile.seekp(0, std::ios::beg);
        IndexPage indexBuffer{0};

        for (int i = 0; i < keys.size(); i++) {
            indexBuffer.keys[i] = keys[i];
            indexBuffer.pages[i] = data.tellp();
            DataPage dataPage{0, -1};
            dataFile.write((char *)&dataPage, sizeof(DataPage));
            indexPage.n++;
        }

        indxFile.write((char *)&indexPage, sizeof(IndexPage));
        indxFile.close();
        dataFile.close();
    };

   private:
    //
    // HELPERS
    //
    template <typename PageType>
    PageType loadPage(int pageToload, std::fstream &f) {
        PageType toReturn{};
        f.seekg(pageToload * TAM_PAGE);
        f.read((char *)&toReturn, sizeof(PageType));
        return toReturn;
    };

    long returnPage(IndexPage &page, const T key) {
        long toReturn = page.pages[0];
        for (int i = 0; i < page.n; i++) {
            if (key > page.keys[i])
                toReturn = page.pages[i + 1];
            else
                break;
        }
        return toReturn;
    };

    long getDataPage(std::fstream &f, const T key) {
        int currentLevel = 0;
        long nextPage = 0;

        // Buscar la pagina en los indexpages
        while (currentLevel < MAX_DEPTH) {
            f.open(idxName(currentLevel), std::ios::binary | std::ios::in);
            IndexPage currentIndexPage = this->loadPage<IndexPage>(nextPage, f);
            f.close();
            nextPage = this->returnPage(currentIndexPage, key);
        }
    }

    bool fileExists(const std::string &name) {
        std::ifstream f(name.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
        return f.good();
    };

    //
    // OPERACIONES
    //
    bool addToPage(const Record &record, long pagePos, std::fstream &f) {
        // Cargar la página
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
            this->addToPage(record, data.nextPage, f);
            return true;
        }

        // Añadir el registro a la pagina
        data.records[data.n] = record;
        data.n++;
        f.seekp(pagePos, std::ios::beg);
        f.write((char *)&data, sizeof(DataPage));
        return true;
    }

    bool searchInPage(const T key, long pagePos, std::vector<Record> &buffer, std::fstream &f) {
        DataPage data = loadPage<DataPage>(pagePos, f);
        bool founded = false;
        bool checkNext = false;
        for (int i = 0; i < data.n; i++) {
            if (data.records[i].id == key ) {
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

    std::string idxName(int i) { 
        return this->filename + std::to_string(i) + ".idx"; 
    };
};