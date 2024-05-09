#include <iostream>
#include <fstream>
#include "db_engine.hpp"

int main() {
    std::cout << "START!" << std::endl;

    //Test db_engine
    Db_engine db(Db_engine::mode::isam,"DATA");


    std::cout << "Done!" << std::endl;
    return 0;
}