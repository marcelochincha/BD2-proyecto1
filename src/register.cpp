#include "register.hpp"

std::ostream& operator<<(std::ostream& os, const Register& r) {
    os << "CustomerID: " << r.CustomerID << std::endl;
    os << "ProductID: " << r.ProductID << std::endl;
    os << "Quantity: " << r.Quantity << std::endl;
    os << "Price: " << r.Price << std::endl;
    os << "TransactionDate: " << r.TransactionDate << std::endl;
    os << "PaymentMethod: " << r.PaymentMethod << std::endl;
    os << "StoreLocation: " << r.StoreLocation << std::endl;
    os << "ProductCategory: " << r.ProductCategory << std::endl;
    os << "DiscountApplied: " << r.DiscountApplied << std::endl;
    os << "TotalAmount: " << r.TotalAmount << std::endl;
    return os;
}

std::string register_to_string(const Register& r) {
    std::string s = std::to_string(r.CustomerID) + " ";
    s += std::string(r.ProductID,1) + " ";
    s += std::to_string(r.Quantity) + " ";
    s += std::to_string(r.Price) + " ";
    s += std::string(r.TransactionDate) + " ";
    s += std::string(r.PaymentMethod) + " ";
    s += std::string(r.StoreLocation) + " ";
    s += std::string(r.ProductCategory) + " ";
    s += std::to_string(r.DiscountApplied) + " ";
    s += std::to_string(r.TotalAmount) + " ";
    return s;
}