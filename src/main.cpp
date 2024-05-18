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

    // TEST ONLY DATA LOADING
    std::cout << sizeof(Register) << std::endl;


    std::cout << "Loading data..." << std::endl;
    Table* tab;

    tab = new Table(Table::mode::isam, "wa");
    
    std::cout << "Loading data..." << std::endl;
    readCSV("dataA.csv", tab);
    std::cout << "LOADED!" << std::endl;

    //Show
    std::cout << "Showing data..." << std::endl;
    std::vector<Register> regs = tab->search(121413);
    for (auto reg : regs) {
        std::cout << reg.CustomerID << " " << reg.ProductID << " " << reg.Quantity << " " << reg.Price << " " << reg.TransactionDate << " " << reg.PaymentMethod << " " << reg.StoreLocation << " " << reg.ProductCategory << " " << reg.DiscountApplied << " " << reg.TotalAmount << std::endl;
    }


    delete tab;

    std::cout << "Done!" << std::endl;
    return 0;
}