#include <iostream>
#include <fstream>
#include "Table.hpp"




int main() {
    std::cout << "START!" << std::endl;

    /*
        //Test db_engine
        Table db(Table::mode::isam,"data");
        
        db.add(Register{1, "Juan", 10.5});
        db.add(Register{2, "Pedro", 20.5});
        db.add(Register{2, "Luis", 50.5});
        db.add(Register{3, "Maria", 30.5});
        db.add(Register{4, "Ana", 40.5});

        std::vector<Register> result = db.search(2);
        for (auto &r : result) {
            std::cout << "FOUND:" << r;
        }

        db.remove(2);

        // comprobar si esta eliminado
        result = db.search(2);
        for (auto &r : result) {
            std::cout << "FOUND:" << r;
        }
    */
        //SQL PARSER TEST
    std::cout << "Done!" << std::endl;
    return 0;
}