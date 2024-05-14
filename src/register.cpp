#include "register.hpp"

std::ostream& operator<<(std::ostream &os, const Register &r){
    os << r.id << " " << r.name << " " << r.value << "\n";
    return os;
}