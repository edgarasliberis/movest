//
// Created by el on 03/01/16.
//

#include <cmath>
#include <iostream>
#include "MVSteg.h"

// Chosen by a fair dice roll
#define THRESH 5
#define MAX_THRESH 31

void MVSteg::initAsEncoder(movest_params *params) {
    Algorithm::initAsEncoder(params);
    if(!(flags & MOVEST_DUMMY_PASS)) {
        datafile.read(&symb, 1);
    }
}

void MVSteg::initAsDecoder(movest_params *params) {
    Algorithm::initAsDecoder(params);
}

void MVSteg::modifyMV(int16_t *mv) {
    int bit = symb >> index;
    if((bit & 1) ^ (*mv & 1)) {
        if(!(flags & MOVEST_DUMMY_PASS)){
            if (*mv > 0) (*mv)--;
            else (*mv)++;
        }
    }
}

void MVSteg::embedToPair(int16_t *mvX, int16_t *mvY) {
    double mvValX = double(*mvX) / 2;
    double mvValY = double(*mvY) / 2;
    double length = std::hypot(mvValX, mvValY);

    if(length < THRESH || abs(*mvX) > MAX_THRESH || abs(*mvY) > MAX_THRESH) return;

    if (abs(*mvX) > abs(*mvY)) modifyMV(mvX);
    else modifyMV(mvY);

    // If the maximal component changed, it will re-embed the data
    // If it didn't, re-embedding into the same component is a no-op
    if (abs(*mvX) > abs(*mvY)) modifyMV(mvX);
    else modifyMV(mvY);

    // Check for shrinkage
    mvValX = double(*mvX) / 2;
    mvValY = double(*mvY) / 2;
    if(std::hypot(mvValX, mvValY) < THRESH
       || abs(*mvX) > MAX_THRESH
       || abs(*mvY) > MAX_THRESH) return;

    index++;
    bitsProcessed++;

    if(index == sizeof(char) * 8) {
        datafile.read(&symb, 1);
        index = 0;
    }
}

void MVSteg::extractFromPair(int16_t mvX, int16_t mvY) {
    double mvValX = double(mvX) / 2;
    double mvValY = double(mvY) / 2;
    double length = std::hypot(mvValX, mvValY);

    if(length < THRESH || abs(mvX) > MAX_THRESH || abs(mvY) > MAX_THRESH) return;

    int16_t val = (abs(mvX) > abs(mvY))? mvX : mvY;
    symb |= (val & 1) << index;
    index++;
    bitsProcessed++;

    if(index == sizeof(char) * 8) {
        datafile.write(&symb, 1);
        symb = 0;
        index = 0;
    }
}
