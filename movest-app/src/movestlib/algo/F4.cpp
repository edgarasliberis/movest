//
// Created by el on 02/01/16.
//

#include "F4.h"

#define ABS(x) (((x) >= 0)? (x) : (-(x)))

bool F4::embedIntoMvComponent(int16_t *mv, int bit) {
    if(*mv == 0) return false;
    int mvbit = (*mv) & 1;

    // If LSBs are the same and *mv is positive, decrease the absolute value of MV
    if(bit != mvbit && *mv > 0 && !(flags & MOVEST_DUMMY_PASS)) {
        (*mv)--;
    }
    // If LSBs are different and *mv is negative, decrease the absolute value of MV
    if(bit == mvbit && *mv < 0 && !(flags & MOVEST_DUMMY_PASS)) {
        (*mv)++;
    }

    return *mv != 0;
}

bool F4::extractFromMvComponent(int16_t val, int *bit) {
    if(val == 0) return false;
    *bit = (val < 0)? ~val : val;
    return true;
}
