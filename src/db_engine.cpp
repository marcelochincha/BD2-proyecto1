#include "Db_engine.hpp"

Db_engine::Db_engine(db_engine::mode type, std::string file_name) {}

Db_engine::~Db_engine() {}

std::string Db_engine::execute(std::string query) {
    return "Not implemented";
}

std::vector<Register> Db_engine::search(TK key) {
    return this->file_m.search(key);
}

std::vector<Register> Db_engine::range_search(TK begin_key, TK end_key) {
    return this->file_m.range_search(begin_key, end_key);
}

bool Db_engine::add(Register data) {
    return this->file_m.add(data);
}

bool Db_engine::remove(TK key) {
    return this->file_m.remove(key);
}