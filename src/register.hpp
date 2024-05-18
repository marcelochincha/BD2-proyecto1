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
    char PaymentMethod[20];
    char StoreLocation[72];
    char ProductCategory[20];
    float DiscountApplied;
    float TotalAmount;
};
typedef int T;
std::ostream &operator<<(std::ostream &os, const Register &r);
std::string register_to_string(const Register& r);
