#include "Ext_Hash.hpp"

// std::string hash_function(int key, int depth) {
int hash_function(int key, int depth) {
    int hashed_key = key % int(std::pow(2, depth));

    // std::bitset<sizeof(size_t) * 64> bit_representation(hashed_key);
    // std::string bit_string = bit_representation.to_string();
    //
    // return bit_string.substr(bit_string.size() - depth);
    return hashed_key;
}

Ext_Hash::Ext_Hash(std::string _filename) : File_manager(_filename), gd(1) {
    this->indexfile = "index_" + this->filename;

    init_file();
}

void Ext_Hash::init_file() {
    std::cout << "Init starting..." << std::endl;
    std::fstream file(this->filename, std::ios::binary | std::ios::app | std::ios::in | std::ios::out);

    file.seekg(0, std::ios::end);
    int bytes = file.tellg();

    if (bytes) {
        std::cout << "Found file. Reading it..." << std::endl;

        file.close();
    } else {
        std::cout << "No file found. Creating it..." << std::endl;
        file.seekp(0, std::ios::beg);

        Bucket bucket;
        file.write((char *)&bucket, sizeof(bucket));
        file.write((char *)&bucket, sizeof(bucket));

        file.close();

        std::fstream index(this->indexfile, std::ios::binary | std::ios::app | std::ios::in | std::ios::out);

        Index_Dir idx;
        index.write((char *)&idx, sizeof(Index_Dir));
        idx.pointer = 1;
        index.write((char *)&idx, sizeof(Index_Dir));

        index.close();
    }
    std::cout << "Init done!" << std::endl;
}

std::vector<Register> Ext_Hash::search(T key) {
    std::fstream index(this->indexfile, std::ios::binary | std::ios::in | std::ios::out);

    std::fstream file(this->filename, std::ios::binary | std::ios::in | std::ios::out);

    int dir_pointer = hash_function(key, this->gd);
    int pointer = get_pointer(index, dir_pointer);

    index.close();

    Bucket bucket = read_bucket(file, pointer);

    std::vector<Register> result;
    for (int i = 0; i < bucket.size; i++) {
        if (bucket.registers[i].CustomerID == key) result.push_back(bucket.registers[i]);
    }
    return result;
}

std::vector<Register> Ext_Hash::rangeSearch(T begin_key, T end_key) {
    throw std::runtime_error("Extendible Hashing does not support range search");
}

bool Ext_Hash::add(Register reg) {
    std::fstream index(this->indexfile, std::ios::binary | std::ios::in | std::ios::out);

    std::fstream file(this->filename, std::ios::binary | std::ios::in | std::ios::out);

    int dir_pointer = hash_function(reg.CustomerID, this->gd);
    int pointer = get_pointer(index, dir_pointer);

    Bucket bucket = read_bucket(file, pointer);
    if (bucket.size < BUCKET_SIZE) {
        bucket.registers[bucket.size] = reg;
        bucket.size++;

        write_bucket(file, pointer, bucket);

    } else {
        if (bucket.ld < this->gd) {
            // split
            bucket.ld += 1;
            Bucket new_bucket(bucket.ld);
        } else {
            // split and grow dir
        }
    }
    index.close();
    file.close();
}

bool Ext_Hash::remove(T key) {}
