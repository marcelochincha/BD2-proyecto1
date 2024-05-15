#include "database.hpp"

#include <cstring>
#include <queue>
#include <sstream>

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

    if (isNumber(tokens.front())) {
        i = std::stoi(tokens.front());
        return true;
    }
    return false;
}

bool assignFloat(float& f, std::queue<std::string>& tokens) {
    std::string s = tokens.front();
    tokens.pop();
    if (isNumber(tokens.front())) {
        f = std::stof(s);
        tokens.pop();
        return true;
    }
    return false;
}

bool assignString(char* c, std::queue<std::string>& tokens) {
    strcpy(c, tokens.front().c_str());
    tokens.pop();
    return true;
}

bool getRegister(Register& r, std::string data) {
    // Remover parentesis
    data = data.substr(1, data.size() - 2);
    std::queue<std::string> tokens = separateBy(data, ',');

    if (assignInt(r.CustomerID, tokens) && assignString(r.ProductID, tokens) && assignInt(r.Quantity, tokens) &&
        assignFloat(r.Price, tokens) && assignString(r.TransactionDate, tokens) &&
        assignString(r.PaymentMethod, tokens) && assignString(r.StoreLocation, tokens) &&
        assignString(r.ProductCategory, tokens) && assignFloat(r.DiscountApplied, tokens) &&
        assignFloat(r.TotalAmount, tokens))
        return true;

    return false;
}

std::queue<std::string>& separateBy(std::string str, char delim) {
    std::queue<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        tokens.push(token);
    }
    return tokens;
}

std::string Database::execute(std::string query) {
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
std::string Database::create_table(std::queue<std::string>& tokens) {
    tokens.pop();  // Remove TABLE

    // Obtener nombre de la tabla
    std::string table_name = tokens.front();

    tokens.pop();
    tokens.pop();  // Remove FROM
    tokens.pop();  // Remove FILE

    // Obtener path del archivo
    std::string file_path = tokens.front();

    tokens.pop();
    tokens.pop();  // Remove USING
    tokens.pop();  // Remove INDEX

    // Obtener tipo de tabla
    std::string table_type = tokens.front();
    tokens.pop();

    // Crear tabla
    Table::mode type;
    if (table_type == "avl") {
        type = Table::mode::avl_tree;
    } else if (table_type == "isam") {
        type = Table::mode::isam;
    } else if (table_type == "ext") {
        type = Table::mode::ext_hash;
    } else {
        return "Invalid table type";
    }
    tables[table_name] = new Table(type, table_name);

    return "Table created " + table_name + ", with data from:" + file_path + " with index" + table_type + ". (SUCCESS)";
}

// Ejemplo
// INSERT INTO table_name VALUES (...)
std::string Database::insert(std::queue<std::string>& tokens) {
    tokens.pop();  // Remove INTO

    // Obtener nombre de la tabla
    std::string table_name = tokens.front();
    tokens.pop();
    tokens.pop();  // Remove VALUES

    // Obtener valores ,(0,0,0,0) el campo entre parentesis debe estar JUNTO
    std::string data = tokens.front();

    // Crear registro
    Register rtemp;

    if (!getRegister(rtemp, data)) return "Error decoding values (ERROR)";

    // Insert values
    if (tables.find(table_name) == tables.end()) {
        return "Table not found";
    }

    Table* table = tables[table_name];

    if (table->add(rtemp)) {
        return "Values inserted (SUCCESS)";
    } else {
        return "Error inserting values (ERROR)";
    }
};

// SELECT * FROM table_name WHERE CustomerID = 1
// SELECT * FROM table_name WHERE CustomerID BETWEEN 1 AND 10
std::string Database::select(std::queue<std::string>& tokens) {
    tokens.pop();  // Remove *
    tokens.pop();  // Remove FROM
    std::string table_name = tokens.front();
    tokens.pop();
    tokens.pop();  // Remove WHERE
    std::string field = tokens.front();
    if (field != "CustomerID") {
        return "Invalid field to search (must be CustomerID)";
    }

    tokens.pop();

    std::string compType = tokens.front();
    tokens.pop();

    std::string res = "";
    Table* table = tables[table_name];
    if (compType == "BETWEEN") {
        tokens.pop();  // Remove BETWEEN
        std::string value1 = tokens.front();
        tokens.pop();
        tokens.pop();  // Remove AND
        std::string value2 = tokens.front();
        tokens.pop();
        if (tables.find(table_name) == tables.end()) {
            return "Table not found";
        }

        std::vector<Register> result = table->range_search(std::stoi(value1), std::stoi(value2));
        std::string res = "";
        for (int i = 0; i < result.size(); i++) {
            res += register_to_string(result[i]) + "\n";
        }

    } else if (compType == "=") {
        std::string value = tokens.front();
        tokens.pop();
        if (tables.find(table_name) == tables.end()) {
            return "Table not found";
        }
        std::vector<Register> result = table->search(std::stoi(value));

        for (int i = 0; i < result.size(); i++) {
            res += register_to_string(result[i]) + "\n";
        }
    } else
        return "Invalid comparison type :" + compType;
    return res;
}

// REMOVE FROM table_name WHERE CustomerID = 1
std::string Database::remove(std::queue<std::string>& tokens) {
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
    
    if(table->remove(std::stoi(value)))
        return "Values removed (SUCCESS)";
    else
        return "Error removing values (FAILURE)";


}