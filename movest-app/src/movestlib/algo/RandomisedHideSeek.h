//
// Created by el398 on 10/12/15.
//

#ifndef MOVEST_RANDOMISEDHIDESEEK_H
#define MOVEST_RANDOMISEDHIDESEEK_H

#include "../algorithm.h"
#include "HideSeek.h"
#include <random>

/**
 * Pair structure to hold "data bit" to "MV number" mapping.
 */
struct Pair {
    ulong bit, mv;
    bool operator<(const Pair &m) const {
        return mv < m.mv;
    }
};

struct AlgOptions {
    char *seed, *seedEnd;
    uint32_t byteCapacity;
};

class RandomisedHideSeek : public HideSeek {
public:
    void initAsEncoder(movest_params *params);
    void initAsDecoder(movest_params *params);
    movest_result finalise();

protected:
    void embedIntoMv(int16_t *mv);
    void extractFromMv(int16_t val);

private:
    uint fileSize, dataSize;
    unsigned char *data;
    Pair *bitToMvMapping;

    void initialiseMapping(const movest_params *params, uint dataSize);
};


#endif //MOVEST_RANDOMISEDHIDESEEK_H
