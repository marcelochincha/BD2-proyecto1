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

// Constructor
Ext_Hash::Ext_Hash(std::string _filename) : File_manager(_filename) {
    if (!fileExists(this->dataName())) {
        // Add index
        this->init_file();
    } else
        this->loaded = true;
}

// Iniciar archivos
void Ext_Hash::init_file() {
    std::cout << "Init starting..." << std::endl;
    // print value of GD and MD_H

    std::fstream file(dataName(), std::ios::binary | std::ios::app | std::ios::in | std::ios::out);

    file.seekg(0, std::ios::end);
    int bytes = file.tellg();

    if (bytes) {
        std::cout << "Found file. Reading it..." << std::endl;
        // leer header free list
        file.seekg(0, std::ios::beg);
        file.read((char *)&this->pos_free_list, sizeof(this->pos_free_list));
        file.close();
    } else {
        std::cout << "No file found. Creating it..." << std::endl;
        file.seekp(0, std::ios::beg);

        // write header
        file.write((char *)&this->pos_free_list, sizeof(this->pos_free_list));
        file.flush();

        // write initial buckets
        DataPage_H bucket(1, 0);
        file.write((char *)&bucket, sizeof(bucket));
        file.flush();
        bucket = DataPage_H(1, 1);
        file.write((char *)&bucket, sizeof(bucket));
        file.flush();

        file.close();

        std::fstream index(idxName(), std::ios::binary | std::ios::app | std::ios::in | std::ios::out);

        for (int i = 0; i < std::pow(2, this->global_depth); i++) {
            Index_Page_H idx;
            if (i % 2 == 0) {
                idx.pointer = 0;
            } else {
                idx.pointer = 1;
            }
            index.write((char *)&idx, sizeof(Index_Page_H));
            index.flush();
        }
        index.close();
    }
    std::cout << "Init done!" << std::endl;
}

//
// OPERACIONES
//
bool Ext_Hash::add(Register reg) {
    // Abre los archivos
    std::fstream index_file(idxName(), std::ios::binary | std::ios::in | std::ios::out);

    std::fstream data_file(dataName(), std::ios::binary | std::ios::in | std::ios::out);

    // Ir al bucket
    int dir_pointer = hash_function(reg.CustomerID, this->global_depth);
    int pointer = get_pointer(index_file, dir_pointer);  // pointer to bucket

    DataPage_H bucket = read_data_page(data_file, pointer);
    if (bucket.size < MD_H) {
        bucket.registers[bucket.size] = reg;
        bucket.size++;
        write_data_page(data_file, bucket, pointer);
    } else if (bucket.local_depth < this->global_depth) {
        // split
        // añadir un bit 0 en la posición local_depth
        // añadir un bit 1 en la posición local_depth del nuevo bucket
        // imprimir registros
        DataPage_H new_bucket(bucket.local_depth, bucket.bits);

        add_bit(bucket, 0);
        add_bit(new_bucket, 1);

        // write new buckets in temp heap

        Register *temp = new Register[bucket.size + 1];
        for (int i = 0; i < bucket.size; i++) {
            temp[i] = bucket.registers[i];
        }
        temp[bucket.size] = reg;
        int length_temp = bucket.size + 1;

        bucket.size = 0;
        new_bucket.size = 0;

        bucket.next = -1;
        new_bucket.next = -1;

        // write new bucket to file
        write_data_page(data_file, bucket, pointer);
        int pointer_new_page = write_new_page(data_file, new_bucket);

        // Re linked index pages to bucket and new bucket
        re_linked_index(index_file, bucket, new_bucket, pointer, pointer_new_page);
        // rehash all elements of temp
        // print size  of temp

        for (int i = 0; i < length_temp; i++) {
            // insert all elements in the new bucket
            add(temp[i]);
        }

        delete[] temp;

    } else {  // overflow
        // create new bucket
        DataPage_H new_bucket(bucket.local_depth, bucket.bits);
        new_bucket.size = 0;
        new_bucket.registers[new_bucket.size] = reg;
        new_bucket.size++;

        // write new bucket to file
        int pointer_new_page = write_new_page(data_file, new_bucket);

        // set pointer to new bucket
        bucket.next = pointer_new_page;
        write_data_page(data_file, bucket, pointer);
    }

    index_file.close();
    data_file.close();
    return true;
}

bool Ext_Hash::remove(T key) {
    // Eliminar solo el primer registro que encuentre
    std::fstream index_file(idxName(), std::ios::binary | std::ios::in | std::ios::out);

    std::fstream data_file(dataName(), std::ios::binary | std::ios::in | std::ios::out);

    int dir_pointer = hash_function(key, this->global_depth);
    int pointer = get_pointer(index_file, dir_pointer);

    // Declare bucket temp
    DataPage_H bucket_prev;

    int pointer_prev = -1;
    int count_overflow = 0;
    bool found = false;

    do {
        DataPage_H bucket = read_data_page(data_file, pointer);
        for (int i = 0; i < bucket.size; i++) {
            if (bucket.registers[i].CustomerID == key) {
                found = true;
                // delete register
                if (bucket.size > 1) {  // if there are more than one register
                    bucket.registers[i] = bucket.registers[bucket.size - 1];
                    bucket.size--;
                    write_data_page(data_file, bucket, pointer);
                } else if (count_overflow == 0 && bucket.next < 0)  // if there is only one register
                {                                                   // no overflow
                    // set next to -1
                    bucket.size = 0;

                    // linked to free list
                    bucket.free_list = this->pos_free_list;
                    this->pos_free_list = pointer;
                    write_data_page(data_file, bucket, pointer);

                    // Mergear and restore index_file to local_depth-1 bits (least significant)
                    re_linked_index_merge(index_file, bucket);
                } else {  // overflow
                    if (count_overflow == 0) {
                        // leer el siguiente bucket, eliminar el siguiente y ponerlo en free list
                        DataPage_H next_bucket = read_data_page(data_file, bucket.next);

                        // overwrite this bucket
                        write_data_page(data_file, next_bucket, pointer);

                        // linked to free list
                        bucket.free_list = this->pos_free_list;
                        this->pos_free_list = pointer;
                        bucket.size = 0;
                        bucket.next = -1;
                        write_data_page(data_file, bucket, pointer);
                    } else {
                        bucket_prev.next = bucket.next;

                        bucket.size = 0;
                        bucket.free_list = this->pos_free_list;
                        this->pos_free_list = pointer;
                        write_data_page(data_file, bucket, pointer);
                        write_data_page(data_file, bucket_prev, pointer_prev);
                    }
                }
            }
        }
        if (bucket.next < 0) break;
        pointer_prev = pointer;
        pointer = bucket.next;
        bucket_prev = bucket;
        count_overflow++;

    } while (true);

    index_file.close();
    data_file.close();
    return found;
}

std::vector<Register> Ext_Hash::search(T key) {
    std::fstream index_file(idxName(), std::ios::binary | std::ios::in | std::ios::out);

    std::fstream data_file(dataName(), std::ios::binary | std::ios::in | std::ios::out);

    int dir_pointer = hash_function(key, this->global_depth);
    int pointer = get_pointer(index_file, dir_pointer);

    index_file.close();

    std::vector<Register> result;
    do {
        DataPage_H bucket = read_data_page(data_file, pointer);
        for (int i = 0; i < bucket.size; i++) {
            if (bucket.registers[i].CustomerID == key) {
                result.push_back(bucket.registers[i]);
            }
        }
        if (bucket.next < 0) break;

        pointer = bucket.next;

    } while (true);

    return result;
}

std::vector<Register> Ext_Hash::range_search(T begin_key, T end_key) {
    std::cout << "Extendible Hashing does not support range search" << std::endl;
    return std::vector<Register>();
}

//
// NAME FILES
//
std::string Ext_Hash::dataName() {
    return this->filename + DAT_EXT;
}

std::string Ext_Hash::idxName() {
    return this->filename + IDX_EXT;
}

//
// HELPERS
//
bool Ext_Hash::fileExists(const std::string &name) {
    std::ifstream f(name.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    return f.good();
}

void Ext_Hash::add_bit(DataPage_H &bucket, int value) {
    bucket.bits.set(bucket.local_depth, value);
    bucket.local_depth++;
}

int Ext_Hash::get_pointer(std::fstream &file, int index) {
    file.seekg(index * sizeof(Index_Page_H), std::ios::beg);
    Index_Page_H idx;
    file.read((char *)&idx, sizeof(Index_Page_H));
    return idx.pointer;
}

// READ AND WRITE DATA PAGE IN FILE DATA
DataPage_H Ext_Hash::read_data_page(std::fstream &file, int index) {
    file.seekg(index * sizeof(DataPage_H) + sizeof(int), std::ios::beg);
    DataPage_H bucket;
    file.read((char *)&bucket, sizeof(DataPage_H));
    return bucket;
}
// To overwrite bucket in file data
void Ext_Hash::write_data_page(std::fstream &file, DataPage_H bucket, int index) {
    file.seekp(index * sizeof(DataPage_H) + sizeof(int), std::ios::beg);
    file.write((char *)&bucket, sizeof(DataPage_H));
    file.flush();
}

// To write new bucket in file data
int Ext_Hash::write_new_page(std::fstream &file, DataPage_H &bucket) {
    // Siguiendo la logica de free list
    if (this->pos_free_list == -1) {
        file.seekp(0, std::ios::end);
        file.write((char *)&bucket, sizeof(DataPage_H));
        file.flush();

        // return pointer to new bucket
        // return ((file.tellp()-sizeof(int)) / sizeof(DataPage_H))-1;
        std::streampos pos = file.tellp();
        std::streamoff offset = pos - std::streamoff(sizeof(int));
        return (offset / std::streamoff(sizeof(DataPage_H))) - 1;
    } else {
        // go to pos_free_list
        DataPage_H temp = read_data_page(file, this->pos_free_list);
        file.seekp(this->pos_free_list * sizeof(DataPage_H) + sizeof(int), std::ios::beg);
        file.write((char *)&bucket, sizeof(DataPage_H));
        file.flush();
        this->pos_free_list = temp.free_list;

        // return ((file.tellp() - sizeof(int)) / sizeof(DataPage_H)) - 1;
        // Calcular la posición de la nueva página
        std::streampos pos = file.tellp();
        std::streamoff offset = pos - std::streamoff(sizeof(int));
        return (offset / std::streamoff(sizeof(DataPage_H))) - 1;
    }
}

void Ext_Hash::re_linked_index(std::fstream &file, DataPage_H &bucket_1, DataPage_H &bucket_2, int pointer1,
                               int pointer2) {
    // matchear bit1 y bit2

    // help_to match
    int sub_j = this->global_depth - bucket_1.local_depth;
    int limit = pow(2, sub_j);

    // 0 in split
    std::string bits_bucket = bucket_1.bits.to_string().substr(sub_j, bucket_1.local_depth);
    // 1 in split
    std::string bits_new_bucket = bucket_2.bits.to_string().substr(sub_j, bucket_2.local_depth);

    for (int i = 0; i < limit; i++) {
        // convertir i a un string de GD - local_depth bits
        std::bitset<GD> i_bit(i);
        std::string i_str = i_bit.to_string().substr(i_bit.size() - sub_j, sub_j);

        // concatenar i_str con [ bits_bucket| bits_new_bucket]
        std::string i_str_1 = i_str + bits_bucket;
        std::string i_str_2 = i_str + bits_new_bucket;

        // covertir i_str_(1,2) a al entero de decimal de su valor binario
        std::bitset<GD> i_str_bit_1(i_str_1);
        int index_1 = i_str_bit_1.to_ulong();

        std::bitset<GD> i_str_bit_2(i_str_2);
        int index_2 = i_str_bit_2.to_ulong();

        // escribir en index_file el nuevo pointer1
        file.seekp(index_1 * sizeof(Index_Page_H), std::ios::beg);
        Index_Page_H idx;
        idx.pointer = pointer1;
        file.write((char *)&idx, sizeof(Index_Page_H));
        file.flush();

        // escribir en index_file el nuevo pointer2
        file.seekp(index_2 * sizeof(Index_Page_H), std::ios::beg);
        idx.pointer = pointer2;
        file.write((char *)&idx, sizeof(Index_Page_H));
        file.flush();
    }
}

// RE LINK INDEX'S POINTER WHEN A BUCKET IS MERGED
void Ext_Hash::re_linked_index_merge(std::fstream &file, DataPage_H &bucket_1) {
    // matchear bit1 y bit2

    // obtain the pointer of the bucket to be merged
    Index_Page_H idx1;
    int index_to_merge = get_index_to_merge(bucket_1);
    file.seekg(index_to_merge * sizeof(DataPage_H), std::ios::beg);
    file.read((char *)&idx1, sizeof(Index_Page_H));
    int pointer_be_persist = idx1.pointer;

    // help_to match
    int sub_j = this->global_depth - bucket_1.local_depth + 1;
    int limit = pow(2, sub_j);

    // merge (delete bit in local_depth) 011 and 111 -> 11
    std::string bits_bucket = bucket_1.bits.to_string().substr(sub_j, bucket_1.local_depth - 1);

    Index_Page_H idx;
    for (int i = 0; i < limit; i++) {
        // convertir i a un string de GD - local_depth bits
        std::bitset<GD> i_bit(i);
        std::string i_str = i_bit.to_string().substr(i_bit.size() - sub_j, sub_j);

        // concatenar i_str con [ bits_bucket| bits_new_bucket]
        std::string i_str_1 = i_str + bits_bucket;

        // covertir i_str_(1,2) a al entero de decimal de su valor binario
        std::bitset<GD> i_str_bit_1(i_str_1);
        int index_1 = i_str_bit_1.to_ulong();

        // escribir en index_file el nuevo pointer1
        file.seekp(index_1 * sizeof(Index_Page_H), std::ios::beg);
        idx.pointer = pointer_be_persist;
        file.write((char *)&idx, sizeof(Index_Page_H));
        file.flush();
    }
}

// get the pointer of the data page to be merged
int Ext_Hash::get_index_to_merge(DataPage_H &bucket) {
    // matchear bit1 y bit2

    // help_to match
    int sub_j = this->global_depth - bucket.local_depth;
    int limit = pow(2, sub_j);

    // Extract the bits of the bucket
    std::string bits_bucket = bucket.bits.to_string().substr(sub_j, bucket.local_depth);

    // verify if the most significant bit is 0 or 1
    if (bits_bucket[0] == '0') {
        bits_bucket[0] = '1';

    } else {
        bits_bucket[0] = '0';
    }

    // convertir string to bitset
    std::bitset<GD> bits_bucket_bit(bits_bucket);
    int index = bits_bucket_bit.to_ulong();

    return index;
}
