#include <iostream>
#include <fstream>
#include "Table.hpp"




int main() {
    std::cout << "START!" << std::endl;
    //Test db_engine
    Table db(Table::mode::isam,"data");
    //TESTING REQUIRED

    //SQL PARSER TEST
    std::cout << "Done!" << std::endl;
    return 0;
}