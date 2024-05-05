#pragma once

#include <string>
#include <vector>

#include "File_manager.hpp"

// Motor principal de la base de datos.
class Db_engine {
   private:
    file_manager file_m;

   public:
    enum mode { avl_tree = 0, isam, ext_hash };

    db_engine(mode type, std::string file_name);
    ~db_engine();
    std::string execute(std::string query);

    // Subrutinas deberian ser privadas. Publicas para pruebas.
    std::vector<Register> search(TK key);
    std::vector<Register> range_search(TK begin_key, TK end_key);
    bool add(Register data);
    bool remove(TK key);
};