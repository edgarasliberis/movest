//
// Created by el on 02/01/16.
//

#ifndef MOVEST_F3_H
#define MOVEST_F3_H

#include "HideSeek.h"

class F3 : public HideSeek {

protected:
    void embedIntoMv(int16_t *mv);
    void extractFromMv(int16_t val);
};


#endif //MOVEST_F3_H
