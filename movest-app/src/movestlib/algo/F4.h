//
// Created by el on 02/01/16.
//

#ifndef MOVEST_F4_H
#define MOVEST_F4_H

#include "HideSeek.h"

/**
 * F4 embedding algorithm. Modifies motion vectors by decrementing
 * its absolute value when LSB doesn't match that of the payload for
 * positive values and when it does match, for negative values.
 */
class F4 : public HideSeek {
public:
    bool embedIntoMvComponent(int16_t *mv, int bit);
    bool extractFromMvComponent(int16_t val, int *bit);
};

#endif //MOVEST_F4_H
