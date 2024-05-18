#include "Table.hpp"

// Table::Table(Table::mode type, std::string file_name) {
//     std::cout << "Creating table..." << std::endl;
//     this->file_m = new Isam(file_name);
// }

Table::Table(Table::mode type, std::string file_name) {
    std::cout << "Creating table..." << std::endl;
    if (type == Table::mode::ext_hash) {
        // this->file_m = new Ext_Hash(file_name);
    } else if (type == Table::mode::isam) {
        this->file_m = new Isam(file_name);
    } else if (type == Table::mode::avl_tree) {
        this->file_m = new AVLFile(file_name);
    } else {
        this->file_m = nullptr;
        throw std::invalid_argument("Invalid mode");
    }
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

bool Table::isLoaded() {
    return this->file_m->loaded;
}

void Table::setLoaded() {
    this->file_m->loaded = true;
}
