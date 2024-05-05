#pragma once
#include "file_manager.hpp"

// Motor principal de la base de datos.
class db_engine {
    enum db_type {
        avl_tree,
        isam,
        ext_hash
    }

    file_manager* file_m;
};