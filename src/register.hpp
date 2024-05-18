#pragma once

#include <iostream>
#include <string>
#include <vector>

const std::vector<std::string> columns = {"CustomerID",      "ProductID",     "Quantity",      "Price",
                                          "TransactionDate", "PaymentMethod", "StoreLocation", "ProductCategory",
                                          "DiscountApplied", "TotalAmount"};

// Registro de datos
struct Register {
    int CustomerID;  // PRIMARY KEY
    char ProductID;
    int Quantity;
    float Price;
    char TransactionDate[20];
    char PaymentMethod[12];
    char StoreLocation[68];
    char ProductCategory[12];
    float DiscountApplied;
    float TotalAmount;
};
struct Register_avl {
    Register reg;
    int left;
    int right;
    int height;

    Register_avl() {
        left = -1;
        right = -1;
        height = 0;
    }

    Register_avl(Register r) {
        reg = r;
        left = -1;
        right = -1;
        height = 0;
    }
};
typedef int T;
std::ostream &operator<<(std::ostream &os, const Register &r);
std::string register_to_string(const Register &r);
