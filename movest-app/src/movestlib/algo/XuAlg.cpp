//
// Created by el on 12/01/16.
//

#include "XuAlg.h"

// Chosen by a fair dice roll
#define THRESH 5
#define MAX_THRESH 30
#define PI 3.14159265

void XuAlg::modifyMv(int16_t *mv, int bit) {
    if((bit & 1) ^ (*mv & 1)) {
        if(!(flags & MOVEST_DUMMY_PASS)){
            if (*mv >= 0) (*mv)++;
            else (*mv)--;
        }
    }
}

bool XuAlg::doEmbedding(int16_t *mvX, int16_t *mvY, int bit) {
    if(!usableMv(*mvX, *mvY)) return false;

    // |Phase| < 90deg is the same as x > 0
    if (*mvX > 0) modifyMv(mvX, bit);
    else modifyMv(mvY, bit);

    // If the usable component changed, it will re-embed the data
    // If it didn't, re-embedding into the same component is a no-op
    if (*mvX > 0) modifyMv(mvX, bit);
    else modifyMv(mvY, bit);

    return usableMv(*mvX, *mvY); // Check for shrinkage (components have an upper bound)
}

bool XuAlg::doExtraction(int16_t mvX, int16_t mvY, int *bit) {
    if(!usableMv(mvX, mvY)) return false;
    *bit = (mvX > 0)? mvX : mvY;
    return true;
}

bool XuAlg::usableMv(int16_t mvX, int16_t mvY) {
    double mvValX = double(mvX) / 2;
    double mvValY = double(mvY) / 2;
    double length = std::hypot(mvValX, mvValY);

    // Mitigate macroblock corruption
    return !(length < THRESH || abs(mvX) > MAX_THRESH || abs(mvY) > MAX_THRESH);
}
