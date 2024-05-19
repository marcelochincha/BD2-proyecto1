#include <fstream>
#include <iostream>

#include "csvLoader.hpp"
#include "database.hpp"

int main() {
    std::cout << "START!" << std::endl;
    Database db;
    while (true) {
        std::string query;
        std::cout << "#?";
        std::getline(std::cin, query);

        if (query == "") {
            continue;
        }

        if (query == "EXIT") {
            break;
        }

        std::cout << db.execute(query) << std::endl;
        std::cout << std::endl;
    }        
    /*
    std::cout << "Loading data..." << std::endl;
    Table* tab;

    tab = new Table(Table::mode::isam, "datitos");
    readCSV("dataA.csv", tab);
    // insert INTO table_name VALUES (1,
    delete tab;*/

    std::cout << "Done!" << std::endl;
    return 0;
}