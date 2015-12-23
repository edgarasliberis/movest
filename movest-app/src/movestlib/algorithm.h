//
// Created by el398 on 07/12/15.
//

#ifndef MOVEST_ALGORITHM_H
#define MOVEST_ALGORITHM_H

#include <fstream>

#include "movest_connector.h"

class Algorithm {
public:
    virtual void encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride) = 0;
    virtual void decode(int16_t (*mvs[2])[2], uint32_t *mbtype_table, int mv_sample_log2, int mb_width, int mb_height,
                            int mv_stride, int mb_stride) = 0;

    virtual void initAsEncoder(movest_params *params);
    virtual void initAsDecoder(movest_params *params);
    virtual movest_result finalise();
protected:
    std::fstream datafile;
    int flags = 0;
    ulong bitsProcessed = 0;
    bool encoder;
};

static Algorithm *algorithm;

#endif //MOVEST_ALGORITHM_H
