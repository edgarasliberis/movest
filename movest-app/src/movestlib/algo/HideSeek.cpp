//
// Created by el398 on 08/12/15.
//

#include "HideSeek.h"

#include <iostream>

void HideSeek::encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride) {
    std::cout << "w: " << mb_width << " h: " << mb_height << std::endl;
    for (int mb_y = 1; mb_y < mb_height; ++mb_y) {
        for (int mb_x = 1; mb_x < mb_width; ++mb_x) {
            int xy = mb_y * mv_stride + mb_x;
            if (mb_type[xy] != 1) {
                for(int comp = 0; comp < 2; ++comp) {
                    index += cond_embed(&mvs[xy][comp], msg[index / 8] >> (index % 8));
                    index = index % (sizeof(char) * 8 * len);
                }
            }
        }
    }
}

void HideSeek::decode(int16_t (*mvs[2])[2], int mv_sample_log2, int mb_width, int mb_height, int mv_stride){
    std::cout << "w: " << mb_width << " h: " << mb_height << std::endl;

    for (int mb_y = 1; mb_y < mb_height; mb_y++) {
        for (int mb_x = 1; mb_x < mb_width; mb_x++) {
            int xy = (mb_x + mb_y * mv_stride) << mv_sample_log2;
            for(int comp = 0; comp < 2; ++comp) {
                int16_t val = mvs[0][xy][comp];

                if (val != 0 && val != 1) {
                    symb |= (val & 1) << index;
                    index++;
                    if(index == 8) {
                        std::cout << "> " << symb << std::endl;
                        symb = 0;
                        index = 0;
                    }
                }
            }
        }
    }
}

int HideSeek::cond_embed(int16_t *mv, int bit) {
    if(*mv != 0 && *mv != 1) {
        if((bit & 1) && !(*mv & 1)) (*mv)++;
        if(!(bit & 1) && (*mv & 1)) (*mv)--;
        return 1;
    }
    return 0;
}