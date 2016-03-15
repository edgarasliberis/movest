//
// Created by el on 12/01/16.
//

#ifndef MOVEST_XUALG_H
#define MOVEST_XUALG_H

#include "MVSteg.h"

class XuAlg : public MVSteg {
public:
    virtual bool doEmbedding(int16_t *mvX, int16_t *mvY, int bit);
    virtual bool doExtraction(int16_t mvX, int16_t mvY, int *bit);
    virtual bool usableMv(int16_t mvX, int16_t mvY);

private:
    void modifyMv(int16_t *mv, int bit);
    static double phase(int16_t mvX, int16_t mvY);
};

#endif //MOVEST_XUALG_H
