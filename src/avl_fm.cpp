#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "file_manager.hpp"

class AVLFile : public File_manager {
private:
    int root;

public:
    AVLFile(std::string filename);
    void insert(Register_avl &record);
    Register find(T value);
    int height();
    void displayInOrder();
    ~AVLFile() {}

    // Implementación de las funciones virtuales de File_manager
    //dejarlo como register 
    std::vector<Register> search(T key) override;
    std::vector<Register> range_search(T begin_key, T end_key) override;
    bool add(Register data) override;
    bool remove(T key) override;

    void display();

private:
    Register find(int pos, T value, std::fstream &file);
    void displayInOrder(std::fstream &file, int &pos);
    int height(Register_avl &node);
    void insert(std::fstream &file, T &value, int &new_pos, int actual_pos, int parent_pos);
    void updateHeader(std::fstream &file, int new_root);
    int get_costumer_id(std::fstream &file, int pos);//sería Costumer ID
    int get_left(std::fstream &file, int pos);
    void write_left(std::fstream &file, int pos, int left);
    int get_right(std::fstream &file, int pos);
    void write_right(std::fstream &file, int pos, int right);
    int get_height(std::fstream &file, int pos);//calcular la alutra o guardarla en nodos
    void write_height(std::fstream &file, int pos, int height);
    bool CheckDuplicate(std::string filename);
    void searchInRange(std::fstream &file, int pos, T low, T high);
    void updateHeight(std::fstream &file, int pos);
    void balance(std::fstream &file, int pos, int parent_pos);
    int balancingFactor(std::fstream &file, int pos);
    void left_rotate(std::fstream &file, int pos, int parent_pos);
    void right_rotate(std::fstream &file, int pos, int parent_pos);

    void search(T key, std::fstream &file, int pos, std::vector<Register> &results);
    void rangeSearch(T begin_key, T end_key, std::fstream &file, int pos, std::vector<Register> &results);
    void remove(std::fstream &file, int &pos, T key);
};

AVLFile::AVLFile(std::string filename) : File_manager(filename){
    std::fstream file(this->filename, std::ios::in | std::ios::out | std::ios::binary);
    file.seekg(0, std::ios::beg);
    int bytes= file.tellg();

    if(bytes==0){
        std::cout<<"Creando archivo"<<std::endl;
        this->root= -1;
        file.write((char *) &this->root, sizeof(int));
    }else{
        file.seekg(0, std::ios::beg);
        std::cout<<"Reading file..."<<std::endl;
        file.read((char *) &this->root, sizeof(int));
    }
    
    std::cout<<"Init complete"<<std::endl;
    file.close();
};





//implementar completo la linea 33
//primary key del registro ( Costumer ID)
//guiarme del insert del p1_bueno

void AVLFile::insert(Register_avl &record) {
    std::fstream file(this->filename, std::ios::binary | std::ios::in | std::ios::out);

    // Insertar en el archivo
    file.seekp(0, std::ios::end);
    int pos = file.tellp();
    pos -= sizeof(int);  // Restar header (root)
    pos /= sizeof(Register);

    file.write((char *)&record, sizeof(Register));

    // Enlazarlo en su posición (left o right), actualizar la altura del nodo, y balancear
    if (this->root == -1) {
        updateHeader(file, pos);
        file.close();
        return;
    }

    // Usar CustomerID en lugar de cod
    insert(file, record.CustomerID, pos, this->root, -1);
    file.close();
}

void AVLFile:: insert(std::fstream &file, T &value, int &new_pos, int actual_pos, int parent_pos){
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
    int current_customer_id = current.CustomerID;

    if (value < current_customer_id) {
        if (current.left == -1) {
            write_left(file, actual_pos, new_pos);
        } else {
            insert(file, value, new_pos, current.left, actual_pos);
        }
    } else if (value > current_customer_id) {
        if (current.right == -1) {
            write_right(file, actual_pos, new_pos);
        } else {
            insert(file, value, new_pos, current.right, actual_pos);
        }
    }

    updateHeight(file, actual_pos);
    balance(file, actual_pos, parent_pos);
}


Register AVLFile::find(T value) {
};

int AVLFile::height() {
}

void AVLFile::displayInOrder() {};

std::vector<Register> AVLFile::search(T key) {};

std::vector<Register> AVLFile::range_search(T begin_key, T end_key) {};

bool AVLFile::add(Register data) {};

bool AVLFile::remove(T key) {};

void AVLFile::search(T key, std::fstream &file, int pos, std::vector<Register> &results) {};

void AVLFile::rangeSearch(T begin_key, T end_key, std::fstream &file, int pos, std::vector<Register> &results) {};//task01

void AVLFile::remove(std::fstream &file, int &pos, T key) {};//task02

void AVLFile::display() {};

bool AVLFile::CheckDuplicate(std::string filename) {};//task 03

void writeFile(std::string filename) {};

void readFile(std::string filename) {};

int main() {};
