//
// Created by el398 on 10/12/15.
//

#ifndef MOVEST_RANDOMISEDHIDESEEK_H
#define MOVEST_RANDOMISEDHIDESEEK_H

#include "../Algorithm.h"
#include "HideSeek.h"
#include <random>

#define SEED_SIZE 16 // Number of bytes to be extracted from the password for PRNG seed.

/**
 * Randomised Hide & Seek algorithm.
 * An implementation of Hide & Seek algorithm, which spreads
 * the payload data uniformly across the motion vector data.
 */
class RandomisedHideSeek : public HideSeek {
public:
    struct AlgOptions {
        uint32_t byteCapacity;
        uint32_t fileSize; // Decoder only
    };

    RandomisedHideSeek(AlgOptions *algOptions);

    void initAsEncoder(movest_params *params);
    void initAsDecoder(movest_params *params);
    unsigned int computeEmbeddingSize(unsigned int dataSize);
    movest_result finalise();

protected:
    virtual void processMvComponentEmbed(int16_t *mv);
    virtual void processMvComponentExtract(int16_t mv);

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
    AlgOptions opt;

    void initialiseMapping(uint dataSize);
    unsigned int eccDataInflation(unsigned int dataSize);
};


#endif //MOVEST_RANDOMISEDHIDESEEK_H
