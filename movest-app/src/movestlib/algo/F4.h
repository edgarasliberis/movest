//
// Created by el on 02/01/16.
//

#ifndef MOVEST_F4_H
#define MOVEST_F4_H

#include "HideSeek.h"

class F4 : public HideSeek {
public:
    bool embedIntoMvComponent(int16_t *mv, int bit);
    bool extractFromMvComponent(int16_t val, int *bit);
};

#endif //MOVEST_F4_H
