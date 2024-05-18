#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "register.hpp"

#define DAT_EXT ".dat"
#define IDX_EXT ".idx"
#define AUX_EXT ".aux"

int keyCmp(T key1, T key2);

// Clase abstracta para el manejo de archivos
class File_manager {
   private:
    // loaded?

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
    bool loaded = false;
};
