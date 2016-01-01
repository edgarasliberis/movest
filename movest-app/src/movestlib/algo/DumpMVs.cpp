//
// Created by el on 01/01/16.
//

#include <iostream>
#include "DumpMVs.h"

void DumpMVs::encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride) {
    std::cerr << "This algorithm does not support encode mode.\n"
                 "Run decoder application to dump MVs." << std::endl;
}

void DumpMVs::decode(int16_t (*mvs[2])[2], uint32_t *mbtype_table, int mv_sample_log2, int mb_width, int mb_height,
                      int mv_stride, int mb_stride) {
    for (int mb_y = 0; mb_y < mb_height; mb_y++) {
        for (int mb_x = 0; mb_x < mb_width; mb_x++) {
            int xy = (mb_x + mb_y * mv_stride) << mv_sample_log2;
            datafile << mvs[0][xy][0] << ", " << mvs[0][xy][1] << "; ";
        }
        datafile << std::endl;
    }
    datafile << std::endl;
}