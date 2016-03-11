//
// Created by el398 on 10/12/15.
//

#ifndef MOVEST_RANDOMISEDHIDESEEK_H
#define MOVEST_RANDOMISEDHIDESEEK_H

#include "../Algorithm.h"
#include "HideSeek.h"
#include <random>

#define SEED_SIZE 16

class RandomisedHideSeek : public HideSeek {
public:
    struct AlgOptions {
        uint32_t byteCapacity;
        uint32_t fileSize; // Decoder only
    };

    void initAsEncoder(movest_params *params);
    void initAsDecoder(movest_params *params);
    movest_result finalise();
    virtual void embedIntoMvComponent(int16_t *mv);
    virtual void extractFromMvComponent(int16_t val);

private:
    /**
     * Pair structure to hold "data bit" to "MV number" mapping.
     */
    struct Pair {
        ulong bit, mv;
        bool operator<(const Pair &m) const {
            return mv < m.mv;
        }
    };

    uint fileSize, dataSize;
    unsigned char *data;
    Pair *bitToMvMapping;

    void initialiseMapping(AlgOptions *algParams, uint dataSize);
};


#endif //MOVEST_RANDOMISEDHIDESEEK_H
