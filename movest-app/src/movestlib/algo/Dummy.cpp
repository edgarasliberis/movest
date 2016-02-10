//
// Created by el on 01/01/16.
//

#include <iostream>
#include "Dummy.h"

void Dummy::encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride) {
    // Dummy Pass
}

void Dummy::decode(int16_t (*mvs[2])[2], uint32_t *mbtype_table, int mv_sample_log2, int mb_width, int mb_height,
                   int mv_stride, int mb_stride) {
    auto &stream = datafile.exposeStream();
    for (int mb_y = 0; mb_y < mb_height; mb_y++) {
        for (int mb_x = 0; mb_x < mb_width; mb_x++) {
            int xy = (mb_x + mb_y * mv_stride) << mv_sample_log2;
            stream << mvs[0][xy][0] << ", " << mvs[0][xy][1] << ", "
                   << int(mbtype_table[mb_x + mb_y * mb_stride] == 1) << "; ";
            bitsProcessed += 2;
        }
        stream << std::endl;
    }
    stream << std::endl;
}

void Dummy::embedToPair(int16_t *mvX, int16_t *mvY) {

}

void Dummy::extractFromPair(int16_t mvX, int16_t mvY) {

}
