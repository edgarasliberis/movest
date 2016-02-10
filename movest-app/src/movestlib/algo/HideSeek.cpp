//
// Created by el398 on 08/12/15.
//

#include <iostream>
#include "HideSeek.h"

void HideSeek::initAsEncoder(movest_params *params) {
    Algorithm::initAsEncoder(params);
    if(!(flags & MOVEST_DUMMY_PASS)) {
        this->getDataToEmbed();
    }
}

void HideSeek::initAsDecoder(movest_params *params) {
    Algorithm::initAsDecoder(params);
}

void HideSeek::embedToPair(int16_t *mvX, int16_t *mvY) {
    if(stopEmbedding) return;
    embedIntoMv(mvX);
    if(stopEmbedding) return;
    embedIntoMv(mvY);
}

void HideSeek::extractFromPair(int16_t mvX, int16_t mvY) {
    extractFromMv(mvX);
    extractFromMv(mvY);
}

void HideSeek::embedIntoMv(int16_t *mv) {
    int bit = symb[index / 8] >> (index % 8);
    // Equivalent to setting the LSB of '*mv' to the one of 'bit'.
    if((bit & 1) && !(*mv & 1) && !(flags & MOVEST_DUMMY_PASS)) (*mv)++;
    if(!(bit & 1) && (*mv & 1) && !(flags & MOVEST_DUMMY_PASS)) (*mv)--;
    ++index;
    ++bitsProcessed;

    this->getDataToEmbed();
}

void HideSeek::extractFromMv(int16_t val) {
    symb[index / 8] |= (val & 1) << (index % 8);
    index++;
    bitsProcessed++;
    this->writeRecoveredData();
}

void HideSeek::getDataToEmbed() {
    if(index == indexLimit) {
        indexLimit = (uint)datafile.read(symb, sizeof(symb)) * 8;
        if(indexLimit == 0) stopEmbedding = true;
        index = 0;
    }
}

void HideSeek::writeRecoveredData() {
    if(index == sizeof(symb) * 8) {
        datafile.write(symb, sizeof(symb));
        std::fill(symb, symb + sizeof(symb), 0);
        index = 0;
    }
}
