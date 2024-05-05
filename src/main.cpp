#include <iostream>
#include <fstream>
#include "Db_engine.hpp"

int main() {
    //Check if file exists
    std::string file_name = "data.dat";
    std::fstream f(file_name, std::ios::in | std::ios::out | std::ios::app);
    if (!f.good()) {
        std::cout << "File does not exist" << std::endl;
        return 1;
    }
    
    std::cout << "File exists" << std::endl;
    

    return 0;
}