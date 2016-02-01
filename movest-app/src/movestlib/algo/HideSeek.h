//
// Created by el398 on 08/12/15.
//

#ifndef MOVEST_HIDESEEK_H
#define MOVEST_HIDESEEK_H

#include "../algorithm.h"

class HideSeek : public Algorithm {
public:
    virtual void embedToPair(int16_t *mvX, int16_t *mvY);
    virtual void extractFromPair(int16_t mvX, int16_t mvY);
    virtual void initAsEncoder(movest_params *params);
    virtual void initAsDecoder(movest_params *params);

protected:
    virtual void extractFromMv(int16_t val);
    virtual void embedIntoMv(int16_t *mv);
    uint index = 0;
    char symb = 0;
    bool stopEmbedding = false;
};

#endif //MOVEST_HIDESEEK_H
