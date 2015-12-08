//
// Created by el398 on 08/12/15.
//

#ifndef MOVEST_HIDESEEK_H
#define MOVEST_HIDESEEK_H

#include "../algorithm.h"

class HideSeek : public Algorithm {
public:
    HideSeek() {}
    void encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride);
    void decode(int16_t (*mvs[2])[2], int mv_sample_log2, int mb_width, int mb_height, int mv_stride);

private:
    const char msg[9] = "itworks!";
    int len = 8;
    int index = 0;
    char symb = 0;

    int cond_embed(int16_t *mv, int bit);
};

#endif //MOVEST_HIDESEEK_H
