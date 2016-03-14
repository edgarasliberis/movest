//
// Created by el on 01/01/16.
//

#include <iostream>
#include "DumpMvs.h"

void DumpMvs::encode(int16_t (*)[2], uint16_t *, int, int, int) {
    // Dummy Pass
}

void DumpMvs::decode(int16_t (*mvs[2])[2], uint32_t *mbtype_table, int mv_sample_log2, int mb_width, int mb_height,
                     int mv_stride, int mb_stride) {
    auto &stream = datafile.exposeStream();
    if(!dimsWritten) {
        stream << mb_height << " " << mb_width << std::endl;
        dimsWritten = true;
    }
    for (int mb_x = 0; mb_x < mb_width; mb_x++) {
        for (int mb_y = 0; mb_y < mb_height; mb_y++) {
            int xy = (mb_x + mb_y * mv_stride) << mv_sample_log2;
            stream << mvs[0][xy][0] << ", " << mvs[0][xy][1] << ", "
                   << int(mbtype_table[mb_x + mb_y * mb_stride] == 1) << ";" << std::endl;
            bitsProcessed += 2;
        }
    }
}

void DumpMvs::embedIntoMv(int16_t *, int16_t *) {
}

void DumpMvs::extractFromMv(int16_t, int16_t) {
}
