//
// Created by el on 06/01/16.
//

#ifndef MOVEST_OUTGUESS1_H
#define MOVEST_OUTGUESS1_H

#include "RandomisedHideSeek.h"

/**
 * Outguess 1.0 algorithm.
 * An implementation of MSteg, which spreads
 * the payload data uniformly across the motion vector data.
 */
class OutGuess1 : public RandomisedHideSeek {
public:
    using RandomisedHideSeek::RandomisedHideSeek;

protected:
    virtual void processMvComponentEmbed(int16_t *mv);
    virtual void processMvComponentExtract(int16_t val);
};

#endif //MOVEST_OUTGUESS1_H
