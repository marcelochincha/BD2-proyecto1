#include <iostream>
#include <fstream>
#include "db_engine.hpp"


std::ostream& operator<<(std::ostream &os, const Register &r) {
    os << r.id << " " << r.name << " " << r.value << "\n";
    return os;
}

int main() {
    std::cout << "START!" << std::endl;

    //Test db_engine
    Db_engine db(Db_engine::mode::isam,"data");
    
    db.add(Register{1, "Juan", 10.5});
    db.add(Register{2, "Pedro", 20.5});
    db.add(Register{2, "Luis", 50.5});
    db.add(Register{3, "Maria", 30.5});
    db.add(Register{4, "Ana", 40.5});

    std::vector<Register> result = db.search(2);
    for (auto &r : result) {
        std::cout << "FOUND:" << r;
    }

    std::cout << "Done!" << std::endl;
    return 0;
}