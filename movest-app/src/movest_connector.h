//
// Created by el398 on 06/12/15.
//

#ifndef MOVEST_CONNECTOR_H
#define MOVEST_CONNECTOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct MovestState {
    const char *file;
};

void movest_init_algorithm(const char *algname);
void movest_encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride);
void movest_decode(int16_t (*mvs[2])[2], int mv_sample_log2, int mb_width, int mb_height, int mv_stride);

#ifdef __cplusplus
}
#endif

#endif //MOVEST_CONNECTOR_H
