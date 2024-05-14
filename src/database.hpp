#include <iostream>
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
};