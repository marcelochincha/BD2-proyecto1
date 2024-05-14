#include "database.hpp"

Database::Database() {}

Database::~Database() {
    for (auto &t : tables) {
        delete t.second;
    }
}

std::string Database::execute(std::string query){
    return "";
}