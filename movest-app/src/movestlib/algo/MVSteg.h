//
// Created by el on 03/01/16.
//

#ifndef MOVEST_MVSTEG_H
#define MOVEST_MVSTEG_H

#include "HideSeek.h"

class MVSteg : public HideSeek {
public:
    virtual void extractFromPair(int16_t mvX, int16_t mvY);
    virtual void embedToPair(int16_t *mvX, int16_t *mvY);

protected:
    virtual void modifyMV(int16_t *mv);
};

#endif //MOVEST_MVSTEG_H
