#pragma once
#include <fstream>
#include <string>
#include <vector>

#define EXT_STR ".dat"

// Registro de datos
struct Register {
    int id;
    char name[15];
    float value;
};

typedef int T;

// Clase abstracta para el manejo de archivos
class File_manager {
   protected:
    std::string filename;
    virtual void init_file() = 0;

   public:
    File_manager(std::string _filename) : filename(_filename){};
    virtual ~File_manager(){};
    virtual std::vector<Register> search(T key) = 0;
    virtual std::vector<Register> range_search(T begin_key, T end_key) = 0;
    virtual bool add(Register data) = 0;
    virtual bool remove(T key) = 0;
};