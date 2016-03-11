//
// Created by el on 01/01/16.
//

#ifndef MOVEST_DUMPMVS_H
#define MOVEST_DUMPMVS_H

#include "../Algorithm.h"

class DumpMvs : public Algorithm {
public:
    virtual void encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride);
    virtual void decode(int16_t (*mvs[2])[2], uint32_t *mbtype_table, int mv_sample_log2, int mb_width, int mb_height,
                int mv_stride, int mb_stride);

    virtual void embedIntoMv(int16_t *mvX, int16_t *mvY);
    virtual void extractFromMv(int16_t mvX, int16_t mvY);

private:
    bool dimsWritten = false;
};

#endif //MOVEST_DUMPMVS_H
