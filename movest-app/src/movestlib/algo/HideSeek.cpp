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

void HideSeek::embedIntoMv(int16_t *mvX, int16_t *mvY) {
    processMvComponentEmbed(mvX);
    processMvComponentEmbed(mvY);
}

void HideSeek::processMvComponentEmbed(int16_t *mv) {
    if(stopEmbedding) return;
    bool success = embedIntoMvComponent(mv, (symb[index / 8] >> (index % 8)) & 1);
    if(success) {
        ++index;
        ++bitsProcessed;
        this->getDataToEmbed();
    }
}

void HideSeek::extractFromMv(int16_t mvX, int16_t mvY) {
    processMvComponentExtract(mvX);
    processMvComponentExtract(mvY);
}

void HideSeek::processMvComponentExtract(int16_t mv) {
    int bit = 0;
    bool success = extractFromMvComponent(mv, &bit);
    if(success) {
        symb[index / 8] |= (bit & 1) << (index % 8);
        index++;
        bitsProcessed++;
        this->writeRecoveredData();
    }
}

bool HideSeek::embedIntoMvComponent(int16_t *mv, int bit) {
    // Equivalent to setting the LSB of '*mv' to the one of 'bit'.
    if((bit & 1) && !(*mv & 1) && !(flags & MOVEST_DUMMY_PASS)) (*mv)++;
    if(!(bit & 1) && (*mv & 1) && !(flags & MOVEST_DUMMY_PASS)) (*mv)--;
    return true;
}

bool HideSeek::extractFromMvComponent(int16_t val, int *bit) {
    *bit = val;
    return true;
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

movest_result HideSeek::finalise() {
    if(!encoder && index > 0 && index / 8 > 0) {
        datafile.write(symb, index / 8);
    }
    return Algorithm::finalise();
}
