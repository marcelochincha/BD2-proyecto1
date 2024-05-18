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
    char ProductID[1];
    int Quantity;
    float Price;
    char TransactionDate[16];
    char PaymentMethod[11];
    char StoreLocation[68];
    char ProductCategory[11];
    float DiscountApplied;
    float TotalAmount;
};
struct Register_avl{
    int CustomerID;  // PRIMARY KEY
    char ProductID[1];
    int Quantity;
    float Price;
    char TransactionDate[16];
    char PaymentMethod[11];
    char StoreLocation[68];
    char ProductCategory[11];
    float DiscountApplied;
    float TotalAmount;
    int left;
    int right;
    int height;
};
typedef int T;
std::ostream &operator<<(std::ostream &os, const Register &r);
std::string register_to_string(const Register& r);
