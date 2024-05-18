#pragma once

#include <string>
#include <vector>

#include "Ext_Hash.hpp"
#include "Isam.hpp"
#include "avl_fm.hpp"
#include "file_manager.hpp"

// Tabla de base de datos
class Table {
   private:
    File_manager* file_m;
    std::string table_name;

   public:
    enum mode { avl_tree, isam, ext_hash, NULL_MODE };

    Table(mode type, std::string table_name);
    ~Table();

    std::string getName();
    std::vector<Register> search(T key);
    std::vector<Register> range_search(T begin_key, T end_key);
    bool add(Register data);
    bool remove(T key);
    bool isLoaded();
    void setLoaded();
};
