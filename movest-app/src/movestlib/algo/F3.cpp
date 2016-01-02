//
// Created by el on 02/01/16.
//

#include "F3.h"

#define ABS(x) (((x) >= 0)? (x) : (-(x)))

void F3::embedIntoMv(int16_t *mv) {
    if(*mv == 0) return;

    int bit = (symb >> index) & 1;
    int mvbit = ABS(*mv) & 1;

    // If LSBs are different, decrease the absolute value of MV
    if((bit ^ mvbit) && !(flags & MOVEST_DUMMY_PASS)) {
        if(*mv > 0) --(*mv);
        else ++(*mv);
    }
    if(*mv != 0) {
        ++index;
        ++bitsProcessed;
    }

    if(index == sizeof(char) * 8) {
        datafile.read(&symb, 1);
        index = 0;
    }
}

void F3::extractFromMv(int16_t val) {
    if(val == 0) return;
    symb |= (ABS(val) & 1) << index;
    index++;
    if(index == sizeof(char) * 8) {
        datafile.write(&symb, 1);
        symb = 0;
        index = 0;
    }
}
