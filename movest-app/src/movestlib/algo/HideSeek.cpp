//
// Created by el398 on 08/12/15.
//

#include "HideSeek.h"

void HideSeek::initAsEncoder(movest_params *params) {
    Algorithm::initAsEncoder(params);
    if(!(flags & MOVEST_DUMMY_PASS)) {
        datafile.read(&symb, 1);
    }
}

void HideSeek::initAsDecoder(movest_params *params) {
    Algorithm::initAsDecoder(params);
}

void HideSeek::embedToPair(int16_t *mvX, int16_t *mvY) {
    embedIntoMv(mvX);
    embedIntoMv(mvY);
}

void HideSeek::extractFromPair(int16_t mvX, int16_t mvY) {
    extractFromMv(mvX);
    extractFromMv(mvY);
}

void HideSeek::embedIntoMv(int16_t *mv) {
    if(stopEmbedding) return;
    int bit = symb >> index;
    // Equivalent to setting the LSB of '*mv' to the one of 'bit'.
    if((bit & 1) && !(*mv & 1) && !(flags & MOVEST_DUMMY_PASS)) (*mv)++;
    if(!(bit & 1) && (*mv & 1) && !(flags & MOVEST_DUMMY_PASS)) (*mv)--;
    ++index;
    ++bitsProcessed;

    if(index == sizeof(char) * 8) {
        if(datafile.eof()) {
            stopEmbedding = true;
        }
        datafile.read(&symb, 1);
        index = 0;
    }
}

void HideSeek::extractFromMv(int16_t val) {
    symb |= (val & 1) << index;
    index++;
    if(index == sizeof(char) * 8) {
        datafile.write(&symb, 1);
        symb = 0;
        index = 0;
    }
}
