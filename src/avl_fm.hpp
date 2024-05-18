#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

#include "file_manager.hpp"

class AVLFile : public File_manager {
   private:
    int root;

   public:
    AVLFile(std::string filename);
    // int height();
    // void displayInOrder();
    ~AVLFile() {}

    // Implementación de las funciones virtuales de File_manager
    // dejarlo como register
    std::vector<Register> search(T key) override;
    std::vector<Register> range_search(T begin_key, T end_key) override;
    bool add(Register data) override;
    bool remove(T key) override;

   private:
    // Register_avl find(int pos, T value, std::fstream &file);
    // void displayInOrder(std::fstream &file, int &pos);
    // int height(Register_avl &node);
    void link_children(std::fstream &file, T &value, int &new_pos, int actual_pos, int parent_pos);
    void updateHeader(std::fstream &file, int new_root);
    int get_customer_id(std::fstream &file, int pos);
    int get_left(std::fstream &file, int pos);
    void write_left(std::fstream &file, int pos, int left);
    int get_right(std::fstream &file, int pos);
    void write_right(std::fstream &file, int pos, int right);
    int get_height(std::fstream &file, int pos);  // calcular la alutra o guardarla en nodos
    void write_height(std::fstream &file, int pos, int height);
    // bool CheckDuplicate(std::string filename);
    // void searchInRange(std::fstream &file, int pos, T low, T high);
    void updateHeight(std::fstream &file, int pos);
    void balance(std::fstream &file, int pos, int parent_pos);
    int balancingFactor(std::fstream &file, int pos);
    void left_rotate(std::fstream &file, int pos, int parent_pos);
    void right_rotate(std::fstream &file, int pos, int parent_pos);

    void search(std::fstream &file, int pos, T key, std::vector<Register> &results);
    void rangeSearch(std::fstream &file, T begin_key, T end_key, int pos, std::vector<Register> &results);
    bool remove(std::fstream &file, int &pos, T key);
    int min_value_node(std::fstream &file, int pos);
};
