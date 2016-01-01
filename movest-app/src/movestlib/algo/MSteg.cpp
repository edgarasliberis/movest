//
// Created by el on 01/01/16.
//

#include "MSteg.h"

void MSteg::embedIntoMv(int16_t *mv) {
    if(*mv != 0 && *mv != 1) {
        HideSeek::embedIntoMv(mv);
    }
}

void MSteg::extractFromMv(int16_t val) {
    if(val != 0 && val != 1) {
        HideSeek::extractFromMv(val);
    }
}
