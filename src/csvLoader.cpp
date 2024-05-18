#include "csvLoader.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Function to read a CSV file and create records
void readCSV(const std::string& filename, Table* tab) {
    if (tab->isLoaded()) {
        std::cout << "Table already loaded" << std::endl;
        return;
    }

    std::cout << filename << std::endl;
    // std::string filepath = "./" + filename;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cout << "Error opening file" << std::endl;
        return;
    }

    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        Register reg;

        std::vector<std::string> tokens;
        while (std::getline(iss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() != 10) {
            std::cerr << "Invalid line: " << line << std::endl;
            return;
        }

        reg.CustomerID = std::stoi(tokens[0]);
        reg.ProductID = tokens[1].c_str()[0];
        reg.Quantity = std::stoi(tokens[2]);
        reg.Price = std::stof(tokens[3]);
        std::strcpy(reg.TransactionDate, tokens[4].c_str());

        std::strcpy(reg.PaymentMethod, tokens[5].c_str());

        std::strcpy(reg.StoreLocation, tokens[6].c_str());
        std::strcpy(reg.ProductCategory, tokens[7].c_str());
        reg.DiscountApplied = std::stof(tokens[8]);
        reg.TotalAmount = std::stof(tokens[9]);

        // Add the record to the table
        tab->add(reg);
    }
    tab->setLoaded();
    file.close();
}
