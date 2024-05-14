#include "table.hpp"


Table::Table(Table::mode type, std::string file_name) {
    std::cout << "Creating table..." << std::endl;
    this->file_m = new Isam(file_name);
}

Table::~Table() {
    std::cout << "Deleting table..." << std::endl;
    delete this->file_m;
}

std::vector<Register> Table::search(T key) {
    return this->file_m->search(key);
}

std::vector<Register> Table::range_search(T begin_key, T end_key) {
    return this->file_m->range_search(begin_key, end_key);
}

bool Table::add(Register data) {
    return this->file_m->add(data);
}

bool Table::remove(T key) {
    return this->file_m->remove(key);
}
