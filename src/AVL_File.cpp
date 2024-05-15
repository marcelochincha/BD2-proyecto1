#include "AFile.hpp"
#include <iostream>

AVLFile::AVLFile(const std::string& filename) : File_manager(filename) {
    init();
}

void AVLFile::init_file() {
    std::fstream file(filename, std::ios::app | std::ios::binary | std::ios::in | std::ios::out);

    file.seekg(0, std::ios::end);
    int bytes = file.tellg();

    if (bytes == 0) {
        std::cout << "Creating file..." << std::endl;
        int root = -1;
        file.write(reinterpret_cast<const char*>(&root), sizeof(int));
    } else {
        file.seekg(0, std::ios::beg);
        std::cout << "Reading file..." << std::endl;
    }

    file.close();
}

// Implementación de las demás funciones miembro de la clase AVLFile...
