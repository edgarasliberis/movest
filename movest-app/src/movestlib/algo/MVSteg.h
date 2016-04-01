//
// Created by el on 03/01/16.
//

#ifndef MOVEST_MVSTEG_H
#define MOVEST_MVSTEG_H

#include "HideSeek.h"
#include "F4.h"

/**
 * MVSteg algorithm.
 * Modifies the largest of the two MV components in an F4 fashion.
 */
class MVSteg : public F4 {
public:
    virtual void extractFromMv(int16_t mvX, int16_t mvY);
    virtual void embedIntoMv(int16_t *mvX, int16_t *mvY);
    virtual bool doEmbedding(int16_t *mvX, int16_t *mvY, int bit);
    virtual bool doExtraction(int16_t mvX, int16_t mvY, int *bit);
    virtual bool usableMv(int16_t mvX, int16_t mvY);
};

#endif //MOVEST_MVSTEG_H
