//
// Created by el on 12/01/16.
//

#include "MVStegVuln.h"
#include <cmath>
#include <iostream>

// Chosen by a fair dice roll
#define THRESH 5
#define MAX_THRESH 30
#define PI 3.14159265

void MVStegVuln::modifyMV(int16_t *mv) {
    int bit = symb >> index;
    if((bit & 1) ^ (*mv & 1)) {
        if(!(flags & MOVEST_DUMMY_PASS)){
            if (*mv > 0) (*mv)++;
            else (*mv)--;
        }
    }
}

void MVStegVuln::embedIntoMv(int16_t *mvX, int16_t *mvY) {
    double mvValX = double(*mvX) / 2;
    double mvValY = double(*mvY) / 2;
    double length = std::hypot(mvValX, mvValY);

    if(length < THRESH || abs(*mvX) > MAX_THRESH || abs(*mvY) > MAX_THRESH) return;

    double angle = fabs(atan2(mvValX, mvValY));
    if (angle < PI / 2) modifyMV(mvX);
    else modifyMV(mvY);

    // If the maximal component changed, it will re-embed the data
    // If it didn't, re-embedding into the same component is a no-op
    mvValX = double(*mvX) / 2;
    mvValY = double(*mvY) / 2;
    angle = fabs(atan2(mvValX, mvValY));
    if (angle < PI / 2) modifyMV(mvX);
    else modifyMV(mvY);

    // Check for shrinkage
    if(abs(*mvX) > MAX_THRESH ||
       abs(*mvY) > MAX_THRESH) return;

    index++;
    bitsProcessed++;

    if(index == sizeof(char) * 8) {
        datafile.read(&symb, 1);
        index = 0;
    }
}

void MVStegVuln::extractFromMv(int16_t mvX, int16_t mvY) {
    double mvValX = double(mvX) / 2;
    double mvValY = double(mvY) / 2;
    double length = std::hypot(mvValX, mvValY);

    if(length < THRESH || abs(mvX) > MAX_THRESH || abs(mvY) > MAX_THRESH) return;

    double angle = fabs(atan2(mvValX, mvValY));
    int16_t val = (angle < PI / 2)? mvX : mvY;
    symb |= (val & 1) << index;
    index++;
    bitsProcessed++;

    if(index == sizeof(char) * 8) {
        datafile.write(&symb, 1);
        symb = 0;
        index = 0;
    }
}
