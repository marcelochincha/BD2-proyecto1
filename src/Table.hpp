#pragma once

#include <string>
#include <vector>

#include "file_manager.hpp"
#include "Isam.hpp"

// Motor principal de la base de datos.
class Table {
   private:
    File_manager* file_m;
    std::string table_name;
   public:
    enum mode { avl_tree = 0, isam, ext_hash, NULL_MODE };

    Table(mode type, std::string table_name);
    ~Table();

    std::string getName();

    // Subrutinas deberian ser privadas. Publicas para pruebas.
    std::vector<Register> search(T key);
    std::vector<Register> range_search(T begin_key, T end_key);
    bool add(Register data);
    bool remove(T key);
};