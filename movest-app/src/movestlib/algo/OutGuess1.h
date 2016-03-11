//
// Created by el on 06/01/16.
//

#ifndef MOVEST_OUTGUESS1_H
#define MOVEST_OUTGUESS1_H

#include "RandomisedHideSeek.h"

class OutGuess1 : public RandomisedHideSeek {
public:
    virtual void embedIntoMvComponent(int16_t *mv);
    virtual void extractFromMvComponent(int16_t val);
};

#endif //MOVEST_OUTGUESS1_H
