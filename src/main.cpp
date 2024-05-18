#include <fstream>
#include <iostream>

#include "csvLoader.hpp"
#include "database.hpp"

int main() {
    std::cout << "START!" << std::endl;

    Database db;
    /*
    while (true) {
        std::string query;
        std::cout << "#?";
        std::getline(std::cin, query);

        if (query == "") {
            continue;
        }

        if (query == "EXIT") {
            break;
        }

        std::cout << db.execute(query) << std::endl;
        std::cout << std::endl;
    }*/        
    
    std::cout << "Loading data..." << std::endl;
    Table* tab;

    tab = new Table(Table::mode::isam, "datitos");
    readCSV("dataA.csv", tab);

    while(true){
        //Show
        int value;
        std::cout << "Enter a value to search: ";
        std::cin >> value;
        std::vector<Register> regs = tab->search(value);
        for (auto reg : regs) {
            std::cout << reg.CustomerID << " " << reg.ProductID << " " << reg.Quantity << " " << reg.Price << " " << reg.TransactionDate << " " << reg.PaymentMethod << " " << reg.StoreLocation << " " << reg.ProductCategory << " " << reg.DiscountApplied << " " << reg.TotalAmount << std::endl;
        }
    }

    delete tab;

    std::cout << "Done!" << std::endl;
    return 0;
}