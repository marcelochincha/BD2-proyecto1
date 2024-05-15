#include "File2_manager.hpp"

File_manager::File_manager(const std::string& _filename) : filename(_filename) {
    init();
}

void File_manager::init() {
    filename += ".dat";
    init_file();
}

// función virtual pura.
