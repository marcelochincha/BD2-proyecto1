#include "avl_fm.hpp"

AVLFile::AVLFile(std::string filename) : File_manager(filename) {
    std::fstream file(this->filename, std::ios::in | std::ios::out | std::ios::binary);
    file.seekg(0, std::ios::beg);
    int bytes = file.tellg();

    if (bytes == 0) {
        std::cout << "Creando archivo" << std::endl;
        this->root = -1;
        file.write((char *)&this->root, sizeof(int));
    } else {
        file.seekg(0, std::ios::beg);
        std::cout << "Reading file..." << std::endl;
        file.read((char *)&this->root, sizeof(int));
    }

    std::cout << "Init complete" << std::endl;
    file.close();
};

// ----------------------------------------------------
std::vector<Register> AVLFile::search(T key) {
    std::vector<Register> records;
    std::fstream file(this->filename, std::ios::binary | std::ios::in | std::ios::out);
    search(file, this->root, key, records);
    file.close();
    return records;
};

std::vector<Register> AVLFile::range_search(T begin_key, T end_key) {
    std::vector<Register> records;
    std::fstream file(this->filename, std::ios::binary | std::ios::in | std::ios::out);
    rangeSearch(file, begin_key, end_key, this->root, records);
    file.close();
    return records;
};

bool AVLFile::add(Register data) {
    std::fstream file(this->filename, std::ios::binary | std::ios::in | std::ios::out);

    // Insertar en el archivo
    file.seekp(0, std::ios::end);
    int pos = file.tellp();
    pos -= sizeof(int);  // Restar header (root)
    pos /= sizeof(Register_avl);

    Register_avl record(data);
    file.write((char *)&record, sizeof(Register_avl));

    // Enlazarlo en su posición (left o right), actualizar la altura del nodo, y balancear
    if (this->root == -1) {
        updateHeader(file, pos);
        file.close();
        return true;
    }

    // Usar CustomerID en lugar de cod
    link_children(file, record.reg.CustomerID, pos, this->root, -1);
    file.close();
    return true;
};

bool AVLFile::remove(T key) {
    std::fstream file(this->filename, std::ios::in | std::ios::out | std::ios::binary);
    bool result = remove(file, this->root, -1, key);
    file.close();
    return result;
};
// ----------------------------------------------------

void AVLFile::updateHeader(std::fstream &file, int new_root) {
    file.seekp(0, std::ios::beg);
    file.write((char *)&new_root, sizeof(int));
};

void AVLFile::link_children(std::fstream &file, T &value, int &new_pos, int actual_pos, int parent_pos) {
    if (actual_pos == -1) {
        if (value < get_customer_id(file, parent_pos)) {
            write_left(file, parent_pos, new_pos);
        } else {
            write_right(file, parent_pos, new_pos);
        }
        return;
    }

    Register_avl current;
    file.seekg(sizeof(int) + actual_pos * sizeof(Register_avl));
    file.read((char *)&current, sizeof(Register_avl));
    int current_customer_id = current.reg.CustomerID;

    if (value < current_customer_id) {
        if (current.left == -1) {
            write_left(file, actual_pos, new_pos);
        } else {
            link_children(file, value, new_pos, current.left, actual_pos);
        }
    } else if (value > current_customer_id) {
        if (current.right == -1) {
            write_right(file, actual_pos, new_pos);
        } else {
            link_children(file, value, new_pos, current.right, actual_pos);
        }
    }

    updateHeight(file, actual_pos);
    balance(file, actual_pos, parent_pos);
}

int AVLFile::get_customer_id(std::fstream &file, int pos) {
    Register_avl record;
    file.seekg(sizeof(int) + pos * sizeof(Register_avl));
    file.read((char *)&record, sizeof(Register_avl));
    return record.reg.CustomerID;
};

void AVLFile::write_left(std::fstream &file, int pos, int left) {
    file.seekp(sizeof(int) + sizeof(Register_avl) * pos + sizeof(Register), std::ios::beg);
    file.write((char *)&left, sizeof(int));
    return;
}

void AVLFile::write_right(std::fstream &file, int pos, int right) {
    file.seekp(sizeof(int) + sizeof(Register_avl) * pos + sizeof(Register) + sizeof(int), std::ios::beg);
    file.write((char *)&right, sizeof(int));
    return;
}

void AVLFile::updateHeight(std::fstream &file, int pos) {
    int left = get_left(file, pos);
    int right = get_right(file, pos);

    if (left != -1) left = get_height(file, left);
    if (right != -1) right = get_height(file, right);

    int new_height = std::max(left, right) + 1;
    write_height(file, pos, new_height);
}

int AVLFile::get_left(std::fstream &file, int pos) {
    int left;
    file.seekg(sizeof(int) + sizeof(Register_avl) + sizeof(Register), std::ios::beg);
    file.read((char *)&left, sizeof(int));
    return left;
}

int AVLFile::get_right(std::fstream &file, int pos) {
    int right;
    file.seekg(sizeof(int) + sizeof(Register_avl) + sizeof(Register) + sizeof(int), std::ios::beg);
    file.read((char *)&right, sizeof(int));
    return right;
}

int AVLFile::get_height(std::fstream &file, int pos) {
    int height;
    file.seekg(sizeof(int) + sizeof(Register_avl) + sizeof(Register) + 2 * sizeof(int), std::ios::beg);
    file.read((char *)&height, sizeof(int));
    return height;
}

void AVLFile::write_height(std::fstream &file, int pos, int height) {
    file.seekp(sizeof(int) + sizeof(Register_avl) + sizeof(Register) + 2 * sizeof(int), std::ios::beg);
    file.write((char *)&height, sizeof(int));
    return;
}

void AVLFile::balance(std::fstream &file, int pos, int parent_pos) {
    int hb = balancingFactor(file, pos);
    if (hb >= 2) {  // cargado por la izquierda
        //<=-1, rotacion doble izquierda-derecha
        if (balancingFactor(file, get_left(file, pos)) <= 1) left_rotate(file, get_left(file, pos), pos);
        right_rotate(file, pos, parent_pos);
    } else if (hb <= -2) {                                     // cargado por la derecha
        if (balancingFactor(file, get_right(file, pos)) >= 1)  //>=1, rotacion doble derecha-izquierda
            right_rotate(file, get_right(file, pos), pos);
        left_rotate(file, pos, parent_pos);
    }
}

int AVLFile::balancingFactor(std::fstream &file, int pos) {
    int left = get_left(file, pos);
    int right = get_right(file, pos);

    if (left != -1) left = get_height(file, left);
    if (right != -1) right = get_height(file, right);

    return left - right;
}

void AVLFile::left_rotate(std::fstream &file, int pos, int parent_pos) {
    int right = get_right(file, pos);  // temp
    write_right(file, pos, get_left(file, right));
    write_left(file, right, pos);
    updateHeight(file, pos);
    updateHeight(file, right);

    if (parent_pos == -1)
        updateHeader(file,
                     right);  // header update cuando parent_post sea el root
    else if (get_left(file, parent_pos) == pos)
        write_left(file, parent_pos, right);
    else
        write_right(file, parent_pos, right);
}

void AVLFile::right_rotate(std::fstream &file, int pos, int parent_pos) {
    int left = get_left(file, pos);
    write_left(file, pos, get_right(file, left));
    write_right(file, left, pos);
    updateHeight(file, pos);
    updateHeight(file, left);

    if (parent_pos == -1)
        updateHeader(file,
                     left);  // header update cuando parent_post sea el root
    else if (get_left(file, parent_pos) == pos)
        write_left(file, parent_pos, left);
    else
        write_right(file, parent_pos, left);
}

void AVLFile::search(std::fstream &file, int pos, T key, std::vector<Register> &results) {
    Register_avl node;
    file.seekg((pos * sizeof(Register_avl)) + sizeof(int), std::ios::beg);
    file.read((char *)&node, sizeof(Register_avl));

    if (node.reg.CustomerID == key)
        results.push_back(node.reg);
    else if (key < node.reg.CustomerID)
        return search(file, node.left, key, results);
    else if (key > node.reg.CustomerID)
        return search(file, node.right, key, results);
    else
        std::cerr << "Error: value not found" << std::endl;
}

void AVLFile::rangeSearch(std::fstream &file, T begin_key, T end_key, int pos, std::vector<Register> &results) {
    Register_avl node;
    file.seekg((pos * sizeof(Register_avl)) + sizeof(int), std::ios::beg);
    file.read((char *)&node, sizeof(Register_avl));

    if (node.reg.CustomerID >= begin_key && node.reg.CustomerID <= end_key) {
        results.push_back(node.reg);
        rangeSearch(file, begin_key, end_key, node.left, results);
        rangeSearch(file, begin_key, end_key, node.right, results);
    } else if (node.reg.CustomerID < begin_key)
        rangeSearch(file, begin_key, end_key, node.right, results);
    else if (node.reg.CustomerID > end_key)
        rangeSearch(file, begin_key, end_key, node.left, results);
}

// TODO
bool AVLFile::remove(std::fstream &file, int pos, int parent_pos, T key) {
    if (pos == -1) {
        return false;  // Clave no encontrada.
    }

    Register_avl current;
    file.seekg(sizeof(int) + pos * sizeof(Register_avl));
    file.read((char *)&current, sizeof(Register_avl));

    if (key < current.reg.CustomerID) {
        return remove(file, current.left, pos, key);
    } else if (key > current.reg.CustomerID) {
        return remove(file, current.right, pos, key);
    } else {
        // Clave encontrada, manejar los casos de eliminación
        if (current.left == -1 || current.right == -1) {
            // Caso 1 o 2: No hijo o un solo hijo
            int temp = (current.left != -1) ? current.left : current.right;
            if (temp == -1) {
                // No tiene hijos, simplemente eliminar

            } else {
                // Copia el hijo al nodo actual y luego elimina el hijo
                file.seekg(sizeof(int) + temp * sizeof(Register_avl));
                Register_avl tempReg;
                file.read((char *)&tempReg, sizeof(Register_avl));
                current = tempReg;  // Copia todos los campos
                file.seekp(sizeof(int) + pos * sizeof(Register_avl));
                file.write((char *)&current, sizeof(Register_avl));
                pos = temp;
            }
        } else {
            // Caso 3: Dos hijos, usar el sucesor inmediato
            int successorPos = min_value_node(file, current.right);
            Register_avl successor;
            file.seekg(sizeof(int) + successorPos * sizeof(Register_avl));
            file.read((char *)&successor, sizeof(Register_avl));

            current.reg.CustomerID = successor.reg.CustomerID;  // Reemplaza con el sucesor
            file.seekp(sizeof(int) + pos * sizeof(Register_avl));
            file.write((char *)&current, sizeof(Register_avl));

            // Elimina el sucesor
            remove(file, current.right, pos, successor.reg.CustomerID);
        }
        updateHeight(file, pos);         // Actualiza la altura del nodo actual
        balance(file, pos, parent_pos);  // Balancea el árbol
        return true;
    }
}

int AVLFile::min_value_node(std::fstream &file, int pos) {
    int currentPos = pos;
    Register_avl current;
    while (currentPos != -1) {
        file.seekg(sizeof(int) + currentPos * sizeof(Register_avl));
        file.read((char *)&current, sizeof(Register_avl));
        if (current.left == -1) {
            break;
        }
        currentPos = current.left;
    }
    return currentPos;
}