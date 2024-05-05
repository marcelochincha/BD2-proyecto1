#include <iostream>

#include "Db_engine.hpp"

int main() {
    db_engine db(db_engine::mode::avl_tree, "test");
    std::cout << "Hello, World!" << std::endl;
    return 0;
}