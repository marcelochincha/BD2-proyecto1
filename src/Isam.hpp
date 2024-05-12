#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "file_manager.hpp"

#define MAX_DEPTH 1
#define PAGE_SIZE 1024

// MD y MI son el número de registros que caben en una página de datos e índice.
const int MI = (PAGE_SIZE - sizeof(int) - sizeof(long)) / ((sizeof(int) + sizeof(long)));
const int MD = (PAGE_SIZE - sizeof(int) - sizeof(long)) / (sizeof(Register));

// Pagina de indices
struct IndexPage {
    int n;
    int keys[MI];
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
    void DEBUG_addIndexData(std::vector<T> keys);

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
    //
    // OPERACIONES
    //
    bool addToPage(const Register &record, long pagePos, std::fstream &f);
    bool searchInPage(const T key, long pagePos, std::vector<Register> &buffer, std::fstream &f);
    bool removeInPage(const T key, long pagePos, std::fstream &f);
};