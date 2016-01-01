//
// Created by el on 01/01/16.
//

#ifndef MOVEST_MSTEG_H
#define MOVEST_MSTEG_H

#include "HideSeek.h"

class MSteg : public HideSeek {

protected:
    void embedIntoMv(int16_t *mv);
    void extractFromMv(int16_t val);
};


#endif //MOVEST_MSTEG_H
