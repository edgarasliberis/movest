//
// Created by el on 02/01/16.
//

#include "F4.h"

#define ABS(x) (((x) >= 0)? (x) : (-(x)))

void F4::embedIntoMv(int16_t *mv) {
    if(*mv == 0) return;

    int bit = (symb >> index) & 1;
    int mvbit = (*mv) & 1;

    // If LSBs are the same and *mv is positive, increase the absolute value of MV
    if(bit != mvbit && *mv > 0 && !(flags & MOVEST_DUMMY_PASS)) {
        --(*mv);
    }
    // If LSBs are different and *mv is negative, decrease the absolute value of MV
    if(bit == mvbit && *mv < 0 && !(flags & MOVEST_DUMMY_PASS)) {
        ++(*mv);
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

void F4::extractFromMv(int16_t val) {
    if(val == 0) return;

    int b = (val < 0)? ~val : val;
    symb |= (b & 1) << index;
    index++;
    if(index == sizeof(char) * 8) {
        datafile.write(&symb, 1);
        symb = 0;
        index = 0;
    }
}
