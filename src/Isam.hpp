#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "consumer_id_data.hpp"
#include "file_manager.hpp"

#define MAX_DEPTH 3
#define PAGE_SIZE_INDEX 1536  //
#define PAGE_SIZE_DATA 4096   // 4KB

// MD y MI son el número de registros que caben en una página de datos e índice.
const int MI = (PAGE_SIZE_INDEX - sizeof(int) - sizeof(long)) / ((sizeof(int) + sizeof(long)));
const int MD = (PAGE_SIZE_DATA - sizeof(int) - sizeof(long)) / (sizeof(Register));

// Pagina de indices
struct IndexPage {
    int n;
    T keys[MI];
    long pages[MI + 1];
};

// Pagina de datos
struct DataPage {
    int n;
    long nextPage;  // -1 si es el ultimo registro, en bytes desde el inicio del archivo
    Register records[MD];
};

// Clase para el manejo de archivos con ISAM
class Isam : public File_manager {
   public:
    Isam(std::string _filename);
    ~Isam();
    std::vector<Register> search(T key);
    std::vector<Register> range_search(T begin_key, T end_key);
    bool add(Register data);
    bool remove(T key);
    // Recibe un array constante
    void DEBUG_addIndexData(const int keys[consumer_id_data_size]);

   protected:
    // Iniciar archivos
    void init_file();

   private:
    //
    // HELPERS
    //
    template <typename PageType>
    PageType loadPage(long pagePosition, std::fstream &f);
    long returnPage(IndexPage &page, const T key);
    long getDataPage(const T key);
    bool fileExists(const std::string &name);
    std::string idxName(int i);
    std::string dataName();
    void showTree();

    //
    // OPERACIONES
    //
    bool addToPage(const Register &record, long pagePos, std::fstream &f);
    bool searchInPage(const T key, long pagePos, std::vector<Register> &buffer, std::fstream &f);
    bool removeInPage(const T key, long pagePos, std::fstream &f);
    bool recursiveRangeSearch(const T begin_key, const T end_key, int depth, long posPage,
                              std::vector<Register> &buffer, std::fstream &f);
    bool rangeSearchInPage(const T begin_key, const T end_key, long pagePos, std::vector<Register> &buffer,
                           std::fstream &f);
};
