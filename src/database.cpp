#include "database.hpp"

#include <cstring>
#include <queue>
#include <sstream>

#include "csvLoader.hpp"

Database::Database() {}

Database::~Database() {
    for (auto& t : tables) {
        delete t.second;
    }
}

bool isNumber(const std::string& s) {
    for (char c : s) {
        if (!(std::isdigit(c) || c == '.')) return false;
    }
    return true;
}

bool assignInt(int& i, std::queue<std::string>& tokens) {
    std::string s = tokens.front();
    tokens.pop();

    if (isNumber(s)) {
        i = std::stoi(s);
        return true;
    }
    std::cout << "Error decoding int: " << s << std::endl;
    return false;
}

bool assignFloat(float& f, std::queue<std::string>& tokens) {
    std::string s = tokens.front();
    tokens.pop();
    if (isNumber(s)) {
        f = std::stof(s);
        return true;
    }
    std::cout << "Error decoding float: " << s << std::endl;
    return false;
}

bool assingChar(char& c, std::queue<std::string>& tokens) {
    std::string s = tokens.front();
    tokens.pop();
    if (s.size() == 1) {
        c = s[0];
        return true;
    }
    std::cout << "Error decoding char: " << s << std::endl;
    return false;
}

bool assignString(char* c, std::queue<std::string>& tokens) {
    strcpy(c, tokens.front().c_str());
    tokens.pop();
    return true;
}

std::queue<std::string> separateBy(std::string str, char delim) {
    std::queue<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        tokens.push(token);
    }
    return tokens;
}

// INSERT INTO table_name VALUES =>(1, "A", 1, 1.0, "A", "A", "A", "A", 1.0, 1.0)<=
bool getRegister(Register& r, std::string data) {
    // Remover parentesis
    std::queue<std::string> tokens = separateBy(data, ',');

    if (assignInt(r.CustomerID, tokens) && assingChar(r.ProductID, tokens) && assignInt(r.Quantity, tokens) &&
        assignFloat(r.Price, tokens) && assignString(r.TransactionDate, tokens) &&
        assignString(r.PaymentMethod, tokens) && assignString(r.StoreLocation, tokens) &&
        assignString(r.ProductCategory, tokens) && assignFloat(r.DiscountApplied, tokens) &&
        assignFloat(r.TotalAmount, tokens))
        return true;

    return false;
}

std::string Database::execute(std::string query) {
    // std::cout << "Executing: " << query << "\n";

    if (query == "") {
        return "Empty command";
    }

    // Tokenize
    // Separar por espacios
    std::queue<std::string> tokens = separateBy(query, ' ');

    // Obtener comando
    std::string command = tokens.front();
    tokens.pop();

    // Ejecutar
    switch (command[0]) {
        case 'C':
            return create_table(tokens);
            break;
        case 'I':
            return insert(tokens);
            break;
        case 'S':
            return select(tokens);
            break;
        case 'D':
            return remove(tokens);
            break;
        default:
            return "Invalid command";
            break;
    }

    return "THIS SHOULD NEVER HAPPEN. ABORT!";
}

// Ejemplo
// CREATE TABLE table_name FROM FILE “C:\data.csv” using index HASH
std::string Database::create_table(std::queue<std::string> tokens) {
    if (tokens.size() < 8) {
        return "Invalid number of arguments (Should have spaces)";
    }
    tokens.pop();  // Remove TABLE

    // Obtener nombre de la tabla
    std::string table_name = tokens.front();

    tokens.pop();
    tokens.pop();  // Remove FROM
    tokens.pop();  // Remove FILE

    // Obtener path del archivo
    std::string file_path = tokens.front();
    std::cout << "Reading file: " << file_path << std::endl;

    tokens.pop();
    tokens.pop();  // Remove USING
    tokens.pop();  // Remove INDEX

    // Obtener tipo de tabla
    std::string table_type = tokens.front();
    tokens.pop();

    // Crear tabla
    Table::mode type;
    Table* tNEW;
    if (table_type == "ISAM") {
        type = Table::mode::isam;
        tNEW = new Table(type, table_name);
    } else if (table_type == "AVL") {
        type = Table::mode::avl_tree;
        tNEW = new Table(type, table_name);
    } else if (table_type == "HASH") {
        type = Table::mode::ext_hash;
        tNEW = new Table(type, table_name);
    } else {
        return "Invalid table type";
    }

    std::cout << "Loading data..." << std::endl;
    // Cargar datos
    readCSV(file_path, tNEW);

    tables[table_name] = tNEW;
    return "Table created " + table_name + ", with data from: " + file_path + " with index " + table_type +
           ". (SUCCESS)";
}

// Ejemplo
// INSERT INTO table_name VALUES (...)
std::string Database::insert(std::queue<std::string> tokens) {
    if (tokens.size() < 4) {
        return "Invalid number of arguments (Should have spaces)";
    }


    tokens.pop();  // Remove INTO

    // Obtener nombre de la tabla
    std::string table_name = tokens.front();
    if (tables.find(table_name) == tables.end()) {
        return "Table not found";
    }

    tokens.pop();
    tokens.pop();  // Remove VALUES

    // Obtener valores ,(0,0,0,0) el campo entre parentesis debe estar JUNTO
    std::string data = tokens.front();
    data = data.substr(1, data.size() - 2); // Remover parentesis

    std::cout << "Data: " << data << std::endl;
    // Crear registro
    Register rtemp;

    if (!getRegister(rtemp, data)) return "Error decoding values (ERROR)";
    Table* table = tables[table_name];

    if (table->add(rtemp)) {
        return "Values inserted (SUCCESS)";
    } else {
        return "Error inserting values (ERROR)";
    }
};

// SELECT * FROM table_name WHERE CustomerID = 1
// SELECT * FROM table_name WHERE CustomerID BETWEEN 1 AND 10
std::string Database::select(std::queue<std::string> tokens) {

    tokens.pop();  // Remove *
    tokens.pop();  // Remove FROM
    std::string table_name = tokens.front();
    if (tables.find(table_name) == tables.end()) {
        return "Table not found";
    }
    Table* table = tables[table_name];

    tokens.pop();  // Remove table_name
    tokens.pop();  // Remove WHERE
    std::string field = tokens.front();
    if (field != "CustomerID") {
        return "Invalid field to search (must be CustomerID)";
    }
    tokens.pop();  // Remove CustomerID

    std::string compType = tokens.front();
    tokens.pop();

    std::string res = "";
    if (compType == "BETWEEN") {
        std::string value1 = tokens.front();
        tokens.pop();

        tokens.pop();  // Remove AND

        std::string value2 = tokens.front();
        tokens.pop();

        std::cout << "Searching between " << value1 << " and " << value2 << std::endl;

        last_results = table->range_search(std::stoi(value1), std::stoi(value2));
    } else if (compType == "=") {
        std::string value = tokens.front();
        tokens.pop();
        last_results = table->search(std::stoi(value));
    } else
        return "Invalid comparison type :" + compType;

    // Print columns
    for (auto word : columns) {
        res += word + " | ";
    }
    res += "\n";

    for (size_t i = 0; i < last_results.size(); i++) {
        res += register_to_string(last_results[i]) + "\n";
    }
    return res;
}

// REMOVE FROM table_name WHERE CustomerID = 1
std::string Database::remove(std::queue<std::string> tokens) {
    if (tokens.size() < 6) {
        return "Invalid number of arguments (Should have spaces)";
    }
    tokens.pop();  // Remove FROM

    std::string table_name = tokens.front();
    tokens.pop();

    tokens.pop();  // Remove WHERE

    // Solo se puede borrar mediante CustomerID
    std::string field = tokens.front();
    tokens.pop();

    if (field != "CustomerID") {
        return "Invalid field to remove (must be CustomerID)";
    }

    std::string compType = tokens.front();
    tokens.pop();

    if (compType != "=") {
        return "Invalid comparison type (must be =)";
    }

    std::string value = tokens.front();
    tokens.pop();

    if (tables.find(table_name) == tables.end()) {
        return "Table not found";
    }

    Table* table = tables[table_name];

    if (table->remove(std::stoi(value)))
        return "Values removed (SUCCESS)";
    else
        return "Error removing values (FAILURE)";
}
