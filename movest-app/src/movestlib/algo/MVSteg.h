//
// Created by el on 03/01/16.
//

#ifndef MOVEST_MVSTEG_H
#define MOVEST_MVSTEG_H

#include "../algorithm.h"

class MVSteg : public Algorithm {
public:
    void initAsEncoder(movest_params *params);
    void initAsDecoder(movest_params *params);

    virtual void extractFromPair(int16_t mvX, int16_t mvY);
    virtual void embedToPair(int16_t *mvX, int16_t *mvY);

protected:
    virtual void modifyMV(int16_t *mv);
    uint index = 0;
    char symb = 0;
};

#endif //MOVEST_MVSTEG_H
