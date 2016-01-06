//
// Created by el on 03/01/16.
//

#include <cmath>
#include <iostream>
#include "MVSteg.h"

#define DEC_MB_TYPE_SKIP 0x0800
// Chosen by a fair dice roll
#define THRESH 5
#define MAX_THRESH 32

void MVSteg::initAsEncoder(movest_params *params) {
    Algorithm::initAsEncoder(params);
    if(!(flags & MOVEST_DUMMY_PASS)) {
        datafile.read(&symb, 1);
    }
}

void MVSteg::initAsDecoder(movest_params *params) {
    Algorithm::initAsDecoder(params);
}

void MVSteg::encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride) {
    for (int mb_y = 0; mb_y < mb_height; ++mb_y) {
        for (int mb_x = 0; mb_x < mb_width; ++mb_x) {
            int xy = mb_y * mv_stride + mb_x;
            if (mb_type[xy] == 2) {
                embedIntoMv(&mvs[xy][0], &mvs[xy][1]);
            }
        }
    }
}

void MVSteg::embedIntoMv(int16_t *mvX, int16_t *mvY) {
    double mvValX = double(*mvX) / 2;
    double mvValY = double(*mvY) / 2;
    double length = std::hypot(mvValX, mvValY);

    if(length < THRESH || abs(*mvX) > MAX_THRESH || abs(*mvY) > MAX_THRESH) return;

    int16_t *mv = (abs(*mvX) > abs(*mvY))? mvX : mvY;

    int bit = symb >> index;
    if((bit & 1) ^ (*mv & 1)) {
        if(abs(*mv) == MAX_THRESH) return;
        if(!(flags & MOVEST_DUMMY_PASS)){
            if (*mv > 0) (*mv)++;
            else (*mv)--;
        }
    }

    index++;
    bitsProcessed++;

    if(index == sizeof(char) * 8) {
        datafile.read(&symb, 1);
        index = 0;
    }
}

void MVSteg::decode(int16_t (*mvs[2])[2], uint32_t *mbtype_table, int mv_sample_log2, int mb_width, int mb_height,
                      int mv_stride, int mb_stride) {
    for (int mb_y = 0; mb_y < mb_height; mb_y++) {
        for (int mb_x = 0; mb_x < mb_width; mb_x++) {
            int xy = (mb_x + mb_y * mv_stride) << mv_sample_log2;
            // Type mismatch
            if(mbtype_table[mb_x + mb_y * mb_stride] != 1) {
                extractFromMv(mvs[0][xy][0], mvs[0][xy][1]);
            }
        }
    }
}

void MVSteg::extractFromMv(int16_t mvX, int16_t mvY) {
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
