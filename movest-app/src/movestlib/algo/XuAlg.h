//
// Created by el on 12/01/16.
//

#ifndef MOVEST_XUALG_H
#define MOVEST_XUALG_H

#include "MVSteg.h"

/**
 * Xu's algorithm.
 * Embeds into x component if MV phase is acute, and into y otherwise.
 * Values are modified by incrementing them until
 * their LSBs match that of the payload.
 */
class XuAlg : public MVSteg {
public:
    virtual bool doEmbedding(int16_t *mvX, int16_t *mvY, int bit);
    virtual bool doExtraction(int16_t mvX, int16_t mvY, int *bit);
    virtual bool usableMv(int16_t mvX, int16_t mvY);

private:
    void modifyMv(int16_t *mv, int bit);
};

#endif //MOVEST_XUALG_H
