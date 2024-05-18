#include "file_manager.hpp"

// Return the substraction of key1 - key2
// This means:
//     0 if key1 == key2
//  >  1 if key1 > key2
//  < -1 if key1 < key2
int keyCmp(T key1, T key2) {
    return key1 - key2;
}