//
// Created by el on 03/01/16.
//

#include <cstdlib>
#include "MVSteg.h"

// Chosen by a fair dice roll
#define THRESH 5

bool MVSteg::doEmbedding(int16_t *mvX, int16_t *mvY, int bit) {
    if(!usableMv(*mvX, *mvY)) return false;
    bool success;

    if (abs(*mvX) > abs(*mvY)) success = F4::embedIntoMvComponent(mvX, bit);
    else success = F4::embedIntoMvComponent(mvY, bit);
    if(!success) return false;

    // If the maximal component changed, it will re-embed the data
    // If it didn't, re-embedding into the same component is a no-op
    if (abs(*mvX) > abs(*mvY)) success = F4::embedIntoMvComponent(mvX, bit);
    else success = F4::embedIntoMvComponent(mvY, bit);

    return success && usableMv(*mvX, *mvY); // Check for shrinkage
}

void MVSteg::embedIntoMv(int16_t *mvX, int16_t *mvY) {
    if(stopEmbedding) return;

    int bit = symb[index / 8] >> (index % 8);
    bool success = doEmbedding(mvX, mvY, bit & 1);
    if(!success) return;

    index++;
    bitsProcessed++;

    this->getDataToEmbed();
}

bool MVSteg::doExtraction(int16_t mvX, int16_t mvY, int *bit) {
    if(!usableMv(mvX, mvY)) return false;
    int16_t val = (abs(mvX) > abs(mvY))? mvX : mvY;
    bool success = F4::extractFromMvComponent(val, bit);
    return success;
}

void MVSteg::extractFromMv(int16_t mvX, int16_t mvY) {
    int bit = 0;
    bool success = doExtraction(mvX, mvY, &bit);
    if(!success) return;

    symb[index / 8] |= (bit & 1) << (index % 8);
    index++;
    bitsProcessed++;

    this->writeRecoveredData();
}

bool MVSteg::usableMv(int16_t mvX, int16_t mvY) {
    double mvValX = double(mvX) / 2;
    double mvValY = double(mvY) / 2;

    return std::hypot(mvValX, mvValY) >= THRESH;
}
