#include "database.hpp"

#include <algorithm>
#include <sstream>
#include "register.hpp"

std::string to_lower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

Database::Database() {
    std::cout << "Creating database..." << std::endl;
}

Database::~Database() {
    std::cout << "Deleting database..." << std::endl;
}

void Database::create_table(std::vector<std::string> tokens) {
    // create table Customer from file “C:\data.csv” using index hash(“DNI”)
    std::string table_name = tokens[2];
    std::string data_path = tokens[5];
    std::string mode = tokens[7];
    std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);

    Table::mode selected_mode = Table::mode::NULL_MODE;

    if (mode == "isam")
        selected_mode = Table::mode::isam;
    else if (mode == "hash")
        selected_mode = Table::mode::ext_hash;
    else if (mode == "avl")
        selected_mode = Table::mode::avl_tree;
    else{
        std::cout << "Invalid mode: " << mode << std::endl;
        return;
    }

    // create table
    Table *table = new Table(selected_mode, "d_" + table_name);
    tables[table_name] = table;
}

void Database::select(std::vector<std::string> tokens) {
    // select * from Customer where DNI = x
    std::string table_name = tokens[3];
    std::string key = tokens[7];

    if (tables.find(table_name) == tables.end()) {
        std::cout << "Table not found" << std::endl;
        return;
    }

    Table *table = tables[table_name];
    std::vector<Register> result = table->search(std::stoi(key));
    for (auto &r : result) {
        std::cout << "FOUND:" << r;
    }
}

void Database::insert(std::vector<std::string> tokens) {
    // insert into Customer values (...)
    std::string table_name = tokens[2];
    std::string key = tokens[4];
}


void Database::remove(std::vector<std::string> tokens) {
    // delete from Customer where DNI = x
    std::string table_name = tokens[2];
    std::string key = tokens[6];
}

void Database::update(std::vector<std::string> tokens) {
    // update Customer set name = "Juan" where DNI = x
    std::string table_name = tokens[1];
    std::string key = tokens[9];
    std::string new_name = tokens[5];
}


void Database::execute(std::string query) {
    // split query by spaces
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(query);
    while (std::getline(tokenStream, token, ' ')) {
        tokens.push_back(token);
    }

    // check first token lowercase
    std::string first_token = to_lower(tokens[0]);

    if (first_token == "create") {
        create_table(tokens);
    } else if (first_token == "select") {
        select(tokens);
    } else if (first_token == "insert") {
        insert(tokens);
    } else if (first_token == "delete") {
        remove(tokens);
    } else {
        std::cout << "Invalid query" << std::endl;
    }
}
