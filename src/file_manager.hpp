#pragma once
#include <fstream>
#include <string>
#include <vector>

// Registro de datos
struct Register {
    int id;
    char name[15];
    float value;
};

typedef int TK;

// Clase abstracta para el manejo de archivos
class File_manager {
   protected:
    std::string filename;

   public:
    File_manager(std::string _filename);

    virtual std::vector<Register> search(TK key) = 0;
    virtual std::vector<Register> range_search(TK begin_key, TK end_key) = 0;
    virtual bool add(Register data) = 0;
    virtual bool remove(TK key) = 0;

   private:
    virtual void init_file() = 0;
};