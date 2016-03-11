//
// Created by el on 12/01/16.
//

#ifndef MOVEST_MVSTEGVULN_H
#define MOVEST_MVSTEGVULN_H

#include "HideSeek.h"

class XuAlg : public HideSeek {
public:
    virtual void extractFromMv(int16_t mvX, int16_t mvY);
    virtual void embedIntoMv(int16_t *mvX, int16_t *mvY);
protected:
    void modifyMV(int16_t *mv);
};

#endif //MOVEST_MVSTEGVULN_H
