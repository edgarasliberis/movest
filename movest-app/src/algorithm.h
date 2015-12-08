//
// Created by el398 on 07/12/15.
//

#ifndef MOVEST_ALGORITHM_H
#define MOVEST_ALGORITHM_H

#include "movest_connector.h"

class Algorithm {
public:
    virtual void encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride) = 0;
    virtual void decode(int16_t (*mvs[2])[2], int mv_sample_log2, int mb_width, int mb_height, int mv_stride) = 0;
};

static Algorithm *algorithm;

#endif //MOVEST_ALGORITHM_H
