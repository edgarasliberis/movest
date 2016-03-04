//
// Created by el on 01/01/16.
//

#include "MSteg.h"

void MSteg::embedIntoMvComponent(int16_t *mv) {
    if(*mv != 0 && *mv != 1) {
        HideSeek::embedIntoMvComponent(mv);
    }
}

void MSteg::extractFromMvComponent(int16_t val) {
    if(val != 0 && val != 1) {
        HideSeek::extractFromMvComponent(val);
    }
}
