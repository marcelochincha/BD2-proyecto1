#pragma once

#include <iostream>


// Registro de datos
struct Register {
    int id;
    char name[15];
    float value;

};
typedef int T;
std::ostream& operator<<(std::ostream &os, const Register &r);