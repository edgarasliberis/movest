//
// Created by el on 06/01/16.
//

#include "OutGuess1.h"

void OutGuess1::processMvComponentEmbed(int16_t *mv) {
    if(*mv != 0 && *mv != 1) {
        RandomisedHideSeek::processMvComponentEmbed(mv);
    }
}

void OutGuess1::processMvComponentExtract(int16_t val) {
    if(val != 0 && val != 1) {
        RandomisedHideSeek::processMvComponentExtract(val);
    }
}