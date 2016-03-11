//
// Created by el on 02/01/16.
//

#ifndef MOVEST_F3_H
#define MOVEST_F3_H

#include "HideSeek.h"

class F3 : public HideSeek {
public:
    void embedIntoMvComponent(int16_t *mv);
    void extractFromMvComponent(int16_t val);
};


#endif //MOVEST_F3_H
