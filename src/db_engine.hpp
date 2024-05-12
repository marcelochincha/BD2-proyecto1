#pragma once

#include <string>
#include <vector>

#include "file_manager.hpp"
#include "Isam.hpp"

// Motor principal de la base de datos.
class Db_engine {
   private:
    File_manager* file_m;

   public:
    enum mode { avl_tree = 0, isam, ext_hash };

    Db_engine(mode type, std::string file_name);
    ~Db_engine();
    std::string execute(std::string query);

    // Subrutinas deberian ser privadas. Publicas para pruebas.
    std::vector<Register> search(T key);
    std::vector<Register> range_search(T begin_key, T end_key);
    bool add(Register data);
    bool remove(T key);
};