//
// Created by el on 12/01/16.
//

#ifndef MOVEST_MVSTEGVULN_H
#define MOVEST_MVSTEGVULN_H

#include "MVSteg.h"

class MVStegVuln : public MVSteg {
protected:
    virtual void modifyMV(int16_t *mv);
    virtual void extractFromPair(int16_t mvX, int16_t mvY);
    virtual void embedToPair(int16_t *mvX, int16_t *mvY);
};

#endif //MOVEST_MVSTEGVULN_H
