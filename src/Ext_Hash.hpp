#include <bitset>
#include <cmath>

#include "file_manager.hpp"
#define BUCKET_SIZE 5

struct Index_Dir {
    int pointer;

    Index_Dir() : pointer(0) {}
    Index_Dir(int _pointer) : pointer(_pointer) {}
};

struct Bucket {
    Register registers[BUCKET_SIZE];
    // relative size
    int size;
    // local depth
    int ld;

    Bucket() : size(0), ld(1) {}
    Bucket(int _ld) : size(0), ld(_ld) {}
};

class Ext_Hash : public File_manager {
    std::string indexfile;
    // global depth
    int gd;

    void init_file();
    //
    int get_pointer(std::fstream &file, int index);
    void set_pointer(std::fstream &file, int index, int value);
    //
    int get_size(std::fstream &file, int index);
    void set_size(std::fstream &file, int index, int value);
    int get_ld(std::fstream &file, int index);
    void set_ld(std::fstream &file, int index, int value);
    //
    Bucket read_bucket(std::fstream &file, int index);
    void write_bucket(std::fstream &file, int index, Bucket bucket);

   public:
    Ext_Hash(std::string _filename);
    std::vector<Register> search(T key);
    std::vector<Register> rangeSearch(T begin_key, T end_key);
    bool add(Register reg);
    bool remove(T key);
};
