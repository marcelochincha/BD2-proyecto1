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

// Clase abstracta para el manejo de archivos
class file_manager {
   private:
    std::string file_name;
   public:
    file_manager() = 0;
    ~file_manager();
    std::vector<Register> search(TK key);
    std::Vector<Register> range_search(TK begin_key, TK end_key);
    bool add(Register data);
    bool remove(TK key);
};