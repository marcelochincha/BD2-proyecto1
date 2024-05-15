#include "register.hpp"

std::ostream& operator<<(std::ostream &os, const Register &r){
    os << r.CustomerID << " ";
    os << r.ProductID << " ";
    os << r.Quantity << " ";
    os << r.Price << " ";
    os << r.TransactionDate << " ";
    os << r.PaymentMethod << " ";
    os << r.StoreLocation << " ";
    os << r.ProductCategory << " ";
    os << r.DiscountApplied << " ";
    os << r.TotalAmount << " ";
    return os;
}

std::string register_to_string(const Register& r){
    std::string s = std::to_string(r.CustomerID) + " ";
    s += std::string(r.ProductID) + " ";
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