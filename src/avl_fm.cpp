#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define DAT_EXT ".dat"

struct Register {
    int id;
    char name[15];
    float value;
};

class File_manager {
protected:
    std::string filename;
    void init() {
        filename += DAT_EXT;
        init_file();
    }
    void init_file() {
        std::fstream file(filename, std::ios::app | std::ios::binary | std::ios::in | std::ios::out);

        file.seekg(0, std::ios::end);
        int bytes = file.tellg();

        if (bytes == 0) {
            std::cout << "Creating file..." << std::endl;
            int root = -1;
            file.write(reinterpret_cast<const char*>(&root), sizeof(int));
        } else {
            file.seekg(0, std::ios::beg);
            std::cout << "Reading file..." << std::endl;
        }

        file.close();
    }

public:
    File_manager(const std::string& _filename) : filename(_filename) {
        init();
    }

    virtual ~File_manager() {}
    virtual std::vector<Register> search(int key) = 0;
    virtual std::vector<Register> range_search(int begin_key, int end_key) = 0;
    virtual bool add(const Register& data) = 0;
    virtual bool remove(int key) = 0;
};

class AVLFile : public File_manager {
private:
    struct Node {
        int key;
        int left;
        int right;
        int height;
    };

    int root; // Almacenar la posición de la raíz del árbol en el archivo

public:
    AVLFile(const std::string& filename) : File_manager(filename), root(-1) {
        std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for reading and writing." << std::endl;
            return;
        }

        file.read(reinterpret_cast<char*>(&root), sizeof(int));
        file.close();
    }

    std::vector<Register> search(int key) override;
    std::vector<Register> range_search(int begin_key, int end_key) override;
    bool add(const Register& data) override;
    bool remove(int key) override;

private:
    void init_file() {
    }

    int find(int key, std::fstream& file, int pos);
    void insert(const Register& data, std::fstream& file, int& new_pos, int actual_pos, int parent_pos);
    void update_header(std::fstream& file, int new_root);
    int get_key(std::fstream& file, int pos);
    int get_left(std::fstream& file, int pos);
    void write_left(std::fstream& file, int pos, int left);
    int get_right(std::fstream& file, int pos);
    void write_right(std::fstream& file, int pos, int right);
    int get_height(std::fstream& file, int pos);
    void write_height(std::fstream& file, int pos, int height);
    void update_height(std::fstream& file, int pos);
    void balance(std::fstream& file, int pos, int parent_pos);
    int balancing_factor(std::fstream& file, int pos);
    void left_rotation(std::fstream& file, int pos, int parent_pos);
    void right_rotation(std::fstream& file, int pos, int parent_pos);
    void search_in_range(std::fstream& file, int pos, int low, int high, std::vector<Register>& results);
};

int AVLFile::find(int key, std::fstream& file, int pos) {
    file.seekg((pos * sizeof(Node)) + sizeof(int), std::ios::beg);
    Node node;
    file.read(reinterpret_cast<char*>(&node), sizeof(Node));

    if (node.key == key) {
        return pos;
    } else if (key < node.key) {
        if (node.left != -1) {
            return find(key, file, node.left);
        }
    } else {
        if (node.right != -1) {
            return find(key, file, node.right);
        }
    }

    return -1;
}

std::vector<Register> AVLFile::search(int key) {
    std::vector<Register> results;
    std::fstream file(filename, std::ios::binary | std::ios::in);

    int pos = find(key, file, root);
    if (pos != -1) {
        file.seekg(pos * sizeof(Node) + sizeof(int), std::ios::beg);
        Register record;
        file.read(reinterpret_cast<char*>(&record), sizeof(Register));
        results.push_back(record);
    }

    file.close();
    return results;
}

int AVLFile::get_key(std::fstream& file, int pos) {
    int key;
    file.seekg(sizeof(Node) * pos + sizeof(int), std::ios::beg);
    file.read(reinterpret_cast<char*>(&key), sizeof(int));
    return key;
}

int AVLFile::get_left(std::fstream& file, int pos) {
    int left;
    file.seekg(sizeof(Node) * pos + 2 * sizeof(int) + sizeof(char[15]), std::ios::beg);
    file.read(reinterpret_cast<char*>(&left), sizeof(int));
    return left;
}

void AVLFile::write_left(std::fstream& file, int pos, int left) {
    file.seekp(sizeof(Node) * pos + 2 * sizeof(int) + sizeof(char[15]), std::ios::beg);
    file.write(reinterpret_cast<const char*>(&left), sizeof(int));
}

int AVLFile::get_right(std::fstream& file, int pos) {
    int right;
    file.seekg(sizeof(Node) * pos + 3 * sizeof(int) + sizeof(char[15]), std::ios::beg);
    file.read(reinterpret_cast<char*>(&right), sizeof(int));
    return right;
}

void AVLFile::write_right(std::fstream& file, int pos, int right) {
    file.seekp(sizeof(Node) * pos + 3 * sizeof(int) + sizeof(char[15]), std::ios::beg);
    file.write(reinterpret_cast<const char*>(&right), sizeof(int));
}

int AVLFile::get_height(std::fstream& file, int pos) {
    int height;
    file.seekg(sizeof(Node) * pos + 4 * sizeof(int) + sizeof(char[15]), std::ios::beg);
    file.read(reinterpret_cast<char*>(&height), sizeof(int));
    return height;
}

void AVLFile::write_height(std::fstream& file, int pos, int height) {
    file.seekp(sizeof(Node) * pos + 4 * sizeof(int) + sizeof(char[15]), std::ios::beg);
    file.write(reinterpret_cast<const char*>(&height), sizeof(int));
}

void AVLFile::update_header(std::fstream& file, int new_root) {
    file.seekp(0, std::ios::beg);
    file.write(reinterpret_cast<const char*>(&new_root), sizeof(int));
}

void AVLFile::update_height(std::fstream& file, int pos) {
    int left_height = get_height(file, get_left(file, pos));
    int right_height = get_height(file, get_right(file, pos));
    int height = (left_height > right_height ? left_height : right_height) + 1;
    write_height(file, pos, height);
}

int AVLFile::balancing_factor(std::fstream& file, int pos) {
    return get_height(file, get_right(file, pos)) - get_height(file, get_left(file, pos));
}

void AVLFile::left_rotation(std::fstream& file, int pos, int parent_pos) {
    int right = get_right(file, pos);
    int right_left = get_left(file, right);

    write_right(file, pos, right_left);
    write_left(file, right, pos);

    if (parent_pos == -1) {
        root = right;
    } else {
        if (get_left(file, parent_pos) == pos) {
            write_left(file, parent_pos, right);
        } else {
            write_right(file, parent_pos, right);
        }
    }

    update_height(file, pos);
    update_height(file, right);
}

void AVLFile::right_rotation(std::fstream& file, int pos, int parent_pos) {
    int left = get_left(file, pos);
    int left_right = get_right(file, left);

    write_left(file, pos, left_right);
    write_right(file, left, pos);

    if (parent_pos == -1) {
        root = left;
    } else {
        if (get_left(file, parent_pos) == pos) {
            write_left(file, parent_pos, left);
        } else {
            write_right(file, parent_pos, left);
        }
    }

    update_height(file, pos);
    update_height(file, left);
}

void AVLFile::balance(std::fstream& file, int pos, int parent_pos) {
    while (pos != -1) {
        update_height(file, pos);
        int bf = balancing_factor(file, pos);

        if (bf == 2) {
            if (balancing_factor(file, get_right(file, pos)) < 0) {
                right_rotation(file, get_right(file, pos), pos);
            }
            left_rotation(file, pos, parent_pos);
        } else if (bf == -2) {
            if (balancing_factor(file, get_left(file, pos)) > 0) {
                left_rotation(file, get_left(file, pos), pos);
            }
            right_rotation(file, pos, parent_pos);
        }

        if (parent_pos == -1) {
            pos = get_left(file, pos);
        } else {
            if (get_left(file, parent_pos) == pos) {
                pos = get_left(file, pos);
            } else {
                pos = get_right(file, pos);
            }
        }
    }
}

void AVLFile::insert(const Register& data, std::fstream& file, int& new_pos, int actual_pos, int parent_pos) {
    if (actual_pos == -1) {
        Node node;
        node.key = data.id;
        node.left = -1;
        node.right = -1;
        node.height = 1;

        file.seekp(0, std::ios::end);
        new_pos = file.tellp() / sizeof(Node);

        if (parent_pos == -1) {
            root = new_pos;
        } else {
            if (get_left(file, parent_pos) == actual_pos) {
                write_left(file, parent_pos, new_pos);
            } else {
                write_right(file, parent_pos, new_pos);
            }
        }

        file.write(reinterpret_cast<const char*>(&node), sizeof(Node));
        file.write(reinterpret_cast<const char*>(&data), sizeof(Register));
    } else if (data.id < get_key(file, actual_pos)) {
        insert(data, file, new_pos, get_left(file, actual_pos), actual_pos);
    } else {
        insert(data, file, new_pos, get_right(file, actual_pos), actual_pos);
    }

    balance(file, actual_pos, parent_pos);
}

bool AVLFile::add(const Register& data) {
    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return false;
    }

    int new_pos;
    insert(data, file, new_pos, root, -1);

    file.close();
    return true;
}

void AVLFile::search_in_range(std::fstream& file, int pos, int low, int high, std::vector<Register>& results) {
    if (pos == -1) return;

    int key = get_key(file, pos);
    if (key >= low && key <= high) {
        file.seekg(sizeof(Node) * pos + sizeof(int), std::ios::beg);
        Register record;
        file.read(reinterpret_cast<char*>(&record), sizeof(Register));
        results.push_back(record);
    }

    if (key > low) {
        search_in_range(file, get_left(file, pos), low, high, results);
    }

    if (key < high) {
        search_in_range(file, get_right(file, pos), low, high, results);
    }
}

std::vector<Register> AVLFile::range_search(int begin_key, int end_key) {
    std::vector<Register> results;
    std::fstream file(filename, std::ios::binary | std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for reading." << std::endl;
        return results;
    }

    search_in_range(file, root, begin_key, end_key, results);

    file.close();
    return results;
}

bool AVLFile::remove(int key) {
    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        std::cerr << "Error: Could not open file." << std::endl;
        return false;
    }

    int parent_pos = -1;
    int pos = find(key, file, root);
    if (pos == -1) {
        return false;  // No está en el registro
    }

    int left = get_left(file, pos);
    int right = get_right(file, pos);

    // Caso 1: El nodo no tiene hijos
    if (left == -1 && right == -1) {
        if (parent_pos == -1) {
            root = -1;  // Actualizar el root
        } else if (get_left(file, parent_pos) == pos) {
            write_left(file, parent_pos, -1);
        } else {
            write_right(file, parent_pos, -1);
        }
    }
    // Caso 2: El nodo tiene un hijo
    else if (left == -1 || right == -1) {
        int child_pos = (left != -1) ? left : right;
        if (parent_pos == -1) {
            root = child_pos;  // Actualizar el root
        } else if (get_left(file, parent_pos) == pos) {
            write_left(file, parent_pos, child_pos);
        } else {
            write_right(file, parent_pos, child_pos);
        }
    }
    // Caso 3: El nodo tiene dos hijos
    else {
        int successor_pos = right;
        int successor_parent_pos = pos;

        while (get_left(file, successor_pos) != -1) {
            successor_parent_pos = successor_pos;
            successor_pos = get_left(file, successor_pos);
        }

        // Elimina el sucesor
        int successor_right = get_right(file, successor_pos);
        if (successor_parent_pos == pos) {
            write_right(file, successor_parent_pos, successor_right);
        } else {
            write_left(file, successor_parent_pos, successor_right);
        }

        // Actualiza el nodo 
        Register successor_data;
        file.seekg(successor_pos * sizeof(Node) + sizeof(int), std::ios::beg);
        file.read(reinterpret_cast<char*>(&successor_data), sizeof(Register));
        file.seekp(pos * sizeof(Node) + sizeof(int), std::ios::beg);
        file.write(reinterpret_cast<const char*>(&successor_data), sizeof(Register));

        // Mueve el sucesor a la posición del nodo a eliminar
        pos = successor_pos;
    }

    // Elimina el nodo
    Node empty_node = {-1, -1, -1, 0};
    file.seekp(pos * sizeof(Node), std::ios::beg);
    file.write(reinterpret_cast<const char*>(&empty_node), sizeof(Node));

    file.close();
    return true;
}

int main() {
    AVLFile avlFile("data");
    Register data;

    // Insertar registros de ejemplo
    for (int i = 1; i <= 10; ++i) {
        data.id = i;
        strcpy(data.name, "Name");
        data.value = i * 1.5;
        avlFile.add(data);
    }

    // Búsqueda de registros
    std::vector<Register> result = avlFile.search(5);
    if (!result.empty()) {
        std::cout << "Registro encontrado: " << result[0].id << " " << result[0].name << " " << result[0].value << std::endl;
    } else {
        std::cout << "Registro no encontrado." << std::endl;
    }

    // Eliminar un registro
    avlFile.remove(5);

    // Búsqueda de registros en un rango
    std::vector<Register> rangeResult = avlFile.range_search(2, 8);
    if (!rangeResult.empty()) {
        std::cout << "Registros en el rango encontrado: " << std::endl;
        for (const auto& record : rangeResult) {
            std::cout << record.id << " " << record.name << " " << record.value << std::endl;
        }
    } else {
        std::cout << "No se encontraron registros en el rango especificado." << std::endl;
    }

    return 0;
}
