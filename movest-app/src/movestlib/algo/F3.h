//
// Created by el on 02/01/16.
//

#ifndef MOVEST_F3_H
#define MOVEST_F3_H

#include "HideSeek.h"

/**
 * F3 embedding algorithm. Modifies motion vectors by decrementing
 * its absolute value when LSB doesn't match that of the payload.
 */
class F3 : public HideSeek {
public:
    virtual bool embedIntoMvComponent(int16_t *mv, int bit);
    virtual bool extractFromMvComponent(int16_t val, int *bit);
};


#endif //MOVEST_F3_H
