//
// Created by el398 on 06/12/15.
//

#ifndef MOVEST_CONNECTOR_H
#define MOVEST_CONNECTOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MOVEST_NO_PARAMS = 0,
    MOVEST_DUMMY_PASS = 1,
    MOVEST_ENABLE_ENCRYPTION = 2
} movest_flags;

typedef struct {
    const char *filename;
    movest_flags flags;
    const char *password;
    void *algParams;
} movest_params;

typedef struct {
    unsigned int bytes_processed;
    int error;
} movest_result;

void movest_init_encoder(movest_params *params);
void movest_init_decoder(movest_params *params);
void movest_init_algorithm(const char *algname);
movest_result movest_finalise();
void movest_encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride);
void movest_decode(int16_t (*mvs[2])[2], uint32_t *mbtype_table, int mv_sample_log2, int mb_width, int mb_height, int mv_stride,
                   int mb_stride);

#ifdef __cplusplus
}
#endif

#endif //MOVEST_CONNECTOR_H
