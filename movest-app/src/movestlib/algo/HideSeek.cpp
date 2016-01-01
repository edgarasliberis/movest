//
// Created by el398 on 08/12/15.
//

#include <iostream>
#include "HideSeek.h"

#define DEC_MB_TYPE_SKIP 0x0800

void HideSeek::initAsEncoder(movest_params *params) {
    Algorithm::initAsEncoder(params);
    if(!(flags & MOVEST_DUMMY_PASS)) {
        datafile.read(&symb, 1);
    }
}

void HideSeek::initAsDecoder(movest_params *params) {
    Algorithm::initAsDecoder(params);
}

void HideSeek::encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride) {
    for (int mb_y = 0; mb_y < mb_height; ++mb_y) {
        for (int mb_x = 0; mb_x < mb_width; ++mb_x) {
            int xy = mb_y * mv_stride + mb_x;
            if (mb_type[xy] == 2) {
                embedIntoMv(&mvs[xy][0]);
                embedIntoMv(&mvs[xy][1]);
            }
        }
    }
}

void HideSeek::embedIntoMv(int16_t *mv) {
    int bit = symb >> index;
    // Equivalent to setting the LSB of '*mv' to the one of 'bit'.
    if((bit & 1) && !(*mv & 1) && !(flags & MOVEST_DUMMY_PASS)) (*mv)++;
    if(!(bit & 1) && (*mv & 1) && !(flags & MOVEST_DUMMY_PASS)) (*mv)--;
    ++index;
    ++bitsProcessed;

    if(index == sizeof(char) * 8) {
        datafile.read(&symb, 1);
        index = 0;
    }
}

void HideSeek::decode(int16_t (*mvs[2])[2], uint32_t *mbtype_table, int mv_sample_log2, int mb_width, int mb_height,
                      int mv_stride, int mb_stride) {
    for (int mb_y = 0; mb_y < mb_height; mb_y++) {
        for (int mb_x = 0; mb_x < mb_width; mb_x++) {
            int xy = (mb_x + mb_y * mv_stride) << mv_sample_log2;
            if(!(mbtype_table[mb_x + mb_y * mb_stride] & DEC_MB_TYPE_SKIP)) {
                extractFromMv(mvs[0][xy][0]);
                extractFromMv(mvs[0][xy][1]);
            }
        }
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
