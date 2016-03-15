//
// Created by el on 02/01/16.
//

#include <cstdlib>

#include "F3.h"

bool F3::embedIntoMvComponent(int16_t *mv, int bit) {
    if(*mv == 0) return false;
    int mvbit = abs(*mv) & 1;

    // If LSBs are different, decrease the absolute value of MV
    if((bit ^ mvbit) && !(flags & MOVEST_DUMMY_PASS)) {
        if(*mv > 0) --(*mv);
        else ++(*mv);
    }
    return *mv != 0;
}

bool F3::extractFromMvComponent(int16_t val, int *bit) {
    if(val == 0) return false;
    *bit = abs(val) & 1;
    return true;
}
