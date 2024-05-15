#pragma once

#include <string>
#include <vector>

struct Register {
    int id;
    char name[15];
    float value;
};

class File_manager {
protected:
    std::string filename;
    void init();
    virtual void init_file() = 0; //pure virtual function

public:
    File_manager(const std::string& _filename);

    virtual ~File_manager() {}
    virtual std::vector<Register> search(int key) = 0;
    virtual std::vector<Register> range_search(int begin_key, int end_key) = 0;
    virtual bool add(const Register& data) = 0;
    virtual bool remove(int key) = 0;
};
