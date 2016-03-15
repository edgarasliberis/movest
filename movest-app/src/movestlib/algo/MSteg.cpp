//
// Created by el on 01/01/16.
//

#include "MSteg.h"

bool MSteg::embedIntoMvComponent(int16_t *mv, int bit) {
    if(*mv != 0 && *mv != 1) {
        return HideSeek::embedIntoMvComponent(mv, bit);
    }
    return false;
}

bool MSteg::extractFromMvComponent(int16_t val, int *bit) {
    if(val != 0 && val != 1) {
        return HideSeek::extractFromMvComponent(val, bit);
    }
    return false;
}
