#pragma once

#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

#include "Table.hpp"

// Clase principal
class Database {
   public:
    Database();
    ~Database();

    std::string execute(std::string query);

   private:
    std::unordered_map<std::string, Table*> tables;

    std::string create_table(std::queue<std::string> tokens);
    std::string insert(std::queue<std::string> tokens);
    std::string select(std::queue<std::string> tokens);
    std::string remove(std::queue<std::string> tokens);
};