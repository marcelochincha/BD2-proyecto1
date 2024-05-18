#include <bitset>
#include <cmath>

#include "file_manager.hpp"

#define GD 16 // Depth global
#define PAGE_SIZE_H 1024

// MD_H  es el número de registros que caben en una página de datos .
const int MD_H = (PAGE_SIZE_H - (3*sizeof(int)) - sizeof(long) -1) / (sizeof(Register));


struct Index_Page_H {
    int pointer;

    Index_Page_H() : pointer(0) {}
    Index_Page_H(int _pointer) : pointer(_pointer) {}
};

struct DataPage_H {
    // relative size
    int size;
    // local depth
    int local_depth;
    // to overflow
    int next;

    // free list pointer
    int free_list;

    // almacenar binario
    std::bitset<GD> bits;

    Register registers[MD_H];
    DataPage_H() : size(0), local_depth(1), next(-1), bits(0), free_list(-1) {}
    DataPage_H(int _ld, std::bitset<GD> _bits) : size(0), local_depth(_ld), next(-1), bits(_bits), free_list(-1) {}
};

class Ext_Hash : public File_manager {
    // To store the file name of the index and data files
    std::string idxName();
    std::string dataName();

    // Header file
    int pos_free_list = -1;
    // global depth
    int global_depth = GD;

    // Iniciar archivos
    void init_file();
    
    
    void set_pointer(std::fstream &file, int index, int value);
    
    int get_size(std::fstream &file, int index);
    void set_size(std::fstream &file, int index, int value);
    int get_ld(std::fstream &file, int index);
    void set_ld(std::fstream &file, int index, int value);

    //
    // HELPERS
    //
    bool fileExists(const std::string &name);
    void add_bit(DataPage_H &bucket, int value);
    int get_pointer(std::fstream &file, int index);

    // READ AND WRITE DATA PAGE IN FILE DATA
    DataPage_H read_data_page(std::fstream &file, int index);
    void write_data_page(std::fstream &file, DataPage_H bucket, int index);

    // WRITE NEW DATA PAGE IN FILE DATA (USING FREE LIST LIFO)
    int write_new_page(std::fstream &file, DataPage_H &bucket);

    // RE LINK INDEX'S POINTER WHEN A BUCKET IS SPLIT
    void re_linked_index(std::fstream &file, DataPage_H &bucket_1, DataPage_H &bucket_2, int pointer1, int pointer2);

    // RE LINK INDEX'S POINTER WHEN A BUCKET IS MERGED
    void re_linked_index_merge(std::fstream &file, DataPage_H &bucket_1, int pointer1 );

   public:
    
    Ext_Hash(std::string _filename);
    //
    // OPERACIONES
    //
    bool add(Register reg);
    bool remove(T key);
    std::vector<Register> search(T key);
    std::vector<Register> rangeSearch(T begin_key, T end_key);
    
};

