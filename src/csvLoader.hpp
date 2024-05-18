#pragma once

#include <vector>

#include "register.hpp"
#include "Table.hpp"

void readCSV(const std::string& filename, Table* tab);
