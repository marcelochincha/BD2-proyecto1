#pragma once

#include <fstream>
#include <vector>
#include "File2_manager.hpp" 

class AVLFile : public File_manager {
private:
    struct Node {
        int key;
        int left;
        int right;
        int height;
    };

public:
    AVLFile(const std::string& filename);

    std::vector<Register> search(int key) override;
    std::vector<Register> range_search(int begin_key, int end_key) override;
    bool add(const Register& data) override;
    bool remove(int key) override;

private:
    void init_file();
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
