//
// Created by el on 06/01/16.
//

#ifndef MOVEST_OUTGUESS1_H
#define MOVEST_OUTGUESS1_H

#include "RandomisedHideSeek.h"

class OutGuess1 : public RandomisedHideSeek {
protected:
    void embedIntoMv(int16_t *mv);
    void extractFromMv(int16_t val);
};

#endif //MOVEST_OUTGUESS1_H
