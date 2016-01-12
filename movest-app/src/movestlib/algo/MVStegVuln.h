//
// Created by el on 12/01/16.
//

#ifndef MOVEST_MVSTEGVULN_H
#define MOVEST_MVSTEGVULN_H

#include "MVSteg.h"

class MVStegVuln : public MVSteg {
protected:
    virtual void modifyMV(int16_t *mv);
    virtual void extractFromMv(int16_t mv_x, int16_t mv_y);
    virtual void embedIntoMv(int16_t *mv_x, int16_t *mv_y);
};

#endif //MOVEST_MVSTEGVULN_H
