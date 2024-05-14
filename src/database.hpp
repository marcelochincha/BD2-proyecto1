#pragma once

#include <unordered_map>
#include <vector>

#include "Table.hpp"

std::string to_lower(std::string str);

class Database {
   private:
    std::unordered_map<std::string, Table*> tables;
    void create_table(std::vector<std::string> tokens);
    void select(std::vector<std::string> tokens);
    void insert(std::vector<std::string> tokens);
    void remove(std::vector<std::string> tokens);
    void update(std::vector<std::string> tokens);

   public:
    Database();
    ~Database();
    void execute(std::string query);
};