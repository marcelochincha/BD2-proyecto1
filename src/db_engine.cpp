#include "Db_engine.hpp"
#include "Isam.hpp"


Db_engine::Db_engine(Db_engine::mode type, std::string file_name) {
    this->file_m = new Isam(file_name);
}

Db_engine::~Db_engine() {
    delete this->file_m;
}

std::string Db_engine::execute(std::string query) {
    return "Not implemented";
}

std::vector<Register> Db_engine::search(T key) {
    return this->file_m->search(key);
}

std::vector<Register> Db_engine::range_search(T begin_key, T end_key) {
    return this->file_m->range_search(begin_key, end_key);
}

bool Db_engine::add(Register data) {
    return this->file_m->add(data);
}

bool Db_engine::remove(T key) {
    return this->file_m->remove(key);
}