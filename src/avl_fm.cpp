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
    virtual void init_file() = 0;

   public:
    File_manager(const std::string& _filename) : filename(_filename + DAT_EXT) {
        init_file();
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

   public:
    AVLFile(const std::string& filename) : File_manager(filename) {}

    std::vector<Register> search(int key) override;
    std::vector<Register> range_search(int begin_key, int end_key) override;
    bool add(const Register& data) override;
    bool remove(int key) override;

   private:
    void init_file() override;
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

void AVLFile::init_file() {
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

bool AVLFile::add(const Register& data) {
    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        std::cerr << "Error: Could not open file." << std::endl;
        return false;
    }

    int new_pos = 0;
    int actual_pos = 0;

    file.seekp(0, std::ios::end);
    int pos = file.tellp();
    pos -= sizeof(int);
    pos /= sizeof(Node);

    file.write(reinterpret_cast<const char*>(&data), sizeof(Register));

    if (pos == -1) {
        update_header(file, new_pos);
        return true;
    }

    insert(data, file, new_pos, actual_pos, -1);
    file.close();
    return true;
}

void AVLFile::insert(const Register& data, std::fstream& file, int& new_pos, int actual_pos, int parent_pos) {
    int current_key = get_key(file, actual_pos);

    if (data.id < current_key) {
        if (get_left(file, actual_pos) == -1) {
            write_left(file, actual_pos, new_pos);
        } else {
            insert(data, file, new_pos, get_left(file, actual_pos), actual_pos);
        }
    } else if (data.id > current_key) {
        if (get_right(file, actual_pos) == -1) {
            write_right(file, actual_pos, new_pos);
        } else {
            insert(data, file, new_pos, get_right(file, actual_pos), actual_pos);
        }
    }

    update_height(file, actual_pos);
    balance(file, actual_pos, parent_pos);
}

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

    int pos = find(key, file, 0);
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
    int left = get_left(file, pos);
    int right = get_right(file, pos);

    int new_height = std::max(get_height(file, left), get_height(file, right)) + 1;
    write_height(file, pos, new_height);
}

int AVLFile::balancing_factor(std::fstream& file, int pos) {
    int left = get_height(file, get_left(file, pos));
    int right = get_height(file, get_right(file, pos));
    return left - right;
}

void AVLFile::balance(std::fstream& file, int pos, int parent_pos) {
    int bf = balancing_factor(file, pos);
    if (bf > 1) {
        if (balancing_factor(file, get_left(file, pos)) < 0) {
            left_rotation(file, get_left(file, pos), pos);
        }
        right_rotation(file, pos, parent_pos);
    } else if (bf < -1) {
        if (balancing_factor(file, get_right(file, pos)) > 0) {
            right_rotation(file, get_right(file, pos), pos);
        }
        left_rotation(file, pos, parent_pos);
    }
}

void AVLFile::left_rotation(std::fstream& file, int pos, int parent_pos) {
    int right = get_right(file, pos);
    write_right(file, pos, get_left(file, right));
    write_left(file, right, pos);
    update_height(file, pos);
    update_height(file, right);

    if (parent_pos == -1) {
        update_header(file, right);
    } else if (get_left(file, parent_pos) == pos) {
        write_left(file, parent_pos, right);
    } else {
        write_right(file, parent_pos, right);
    }
}

void AVLFile::right_rotation(std::fstream& file, int pos, int parent_pos) {
    int left = get_left(file, pos);
    write_left(file, pos, get_right(file, left));
    write_right(file, left, pos);
    update_height(file, pos);
    update_height(file, left);

    if (parent_pos == -1) {
        update_header(file, left);
    } else if (get_left(file, parent_pos) == pos) {
        write_left(file, parent_pos, left);
    } else {
        write_right(file, parent_pos, left);
    }
}

std::vector<Register> AVLFile::range_search(int begin_key, int end_key) {
    std::vector<Register> results;
    std::fstream file(filename, std::ios::binary | std::ios::in);

    search_in_range(file, 0, begin_key, end_key, results);

    file.close();
    return results;
}

void AVLFile::search_in_range(std::fstream& file, int pos, int low, int high, std::vector<Register>& results) {
    if (pos == -1) {
        return;
    }

    Node node;
    file.seekg(pos * sizeof(Node) + sizeof(int), std::ios::beg);
    file.read(reinterpret_cast<char*>(&node), sizeof(Node));

    // Recorrer el subarbol izquierdo 
    if (node.key > low) {
        search_in_range(file, node.left, low, high, results);
    }
    if (node.key >= low && node.key <= high) {
        Register record;
        file.seekg(pos * sizeof(Node) + sizeof(int), std::ios::beg);
        file.read(reinterpret_cast<char*>(&record), sizeof(Register));
        results.push_back(record);
    }

    if (node.key < high) {
        search_in_range(file, node.right, low, high, results);
    }
}

bool AVLFile::remove(int key) {
    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        std::cerr << "Error: Could not open file." << std::endl;
        return false;
    }

    int parent_pos = -1;
    int pos = find(key, file, 0);
    if (pos == -1) {
        return false;  // No esta en el registro
    }

    int current_key = get_key(file, pos);
    int left = get_left(file, pos);
    int right = get_right(file, pos);

    // Caso 1: El nodo no tiene hijos
    if (left == -1 && right == -1) {
        if (parent_pos == -1) {
            update_header(file, -1);  // Actualizar el root
        } else if (get_left(file, parent_pos) == pos) {
            write_left(file, parent_pos, -1);
        } else {
            write_right(file, parent_pos, -1);
        }
    }
    // Caso 2: El nodo tiene un hijo
    else if (left == -1 || right == -1) {
        int child = (left == -1) ? right : left;
        if (parent_pos == -1) {
            update_header(file, child);  // Actualizar el root
        } else if (get_left(file, parent_pos) == pos) {
            write_left(file, parent_pos, child);
        } else {
            write_right(file, parent_pos, child);
        }
    }
    // Caso 3: El nodo tiene dos hijos
    else {
        int successor_pos = right;
        while (get_left(file, successor_pos) != -1) {
            successor_pos = get_left(file, successor_pos);
        }

        int successor_key = get_key(file, successor_pos);
        remove(successor_key);  // Eliminar el sucesor
        write_height(file, pos, get_height(file, pos));  // Actualizar la altura del nodo eliminado
        balance(file, pos, parent_pos);  
    }

    file.close();
    return true;
}


int main() {
    std::cout << "START!" << std::endl;
    AVLFile file("MYDAT");
    Register r1{1, "MARCELO1", 1.0f};
    Register r2{2, "MARCELO2", 2.0f};
    Register r3{3, "MARCELO3", 3.0f};
    Register r4{4, "MARCELO4", 4.0f};
    Register r5{5, "MARCELO5", 5.0f};
    Register r6{6, "MARCELO6", 6.0f};
    Register r7{7, "MARCELO7", 7.0f};
    Register r8{8, "MARCELO8", 8.0f};
    Register r9{9, "MARCELO9", 9.0f};

    file.add(r1);
    file.add(r3);
    file.add(r2);
    file.add(r4);
    file.add(r9);
    file.add(r7);
    file.add(r5);
    file.add(r6);
    file.add(r8);


    std::cout << "OK!" << std::endl;
    return 0;
}
