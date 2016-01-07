//
// Created by el on 03/01/16.
//

#ifndef MOVEST_MVSTEG_H
#define MOVEST_MVSTEG_H

#include "../algorithm.h"

class MVSteg : public Algorithm {
public:
    void encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride);
    void decode(int16_t (*mvs[2])[2], uint32_t *mbtype_table, int mv_sample_log2, int mb_width, int mb_height,
                int mv_stride, int mb_stride);
    void initAsEncoder(movest_params *params);
    void initAsDecoder(movest_params *params);

protected:
    void modifyMV(int16_t *mv);
    virtual void extractFromMv(int16_t mv_x, int16_t mv_y);
    virtual void embedIntoMv(int16_t *mv_x, int16_t *mv_y);
    uint index = 0;
    char symb = 0;
};

#endif //MOVEST_MVSTEG_H
