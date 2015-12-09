//
// Created by el398 on 07/12/15.
//

#include <iostream>
#include <cstring>

#include "algorithm.h"
#include "algo/HideSeek.h"

void Algorithm::initAsEncoder(const char *filename) {
    datafile.open(filename, std::ios::in | std::ios::binary);
}

void Algorithm::initAsDecoder(const char *filename) {
    datafile.open(filename, std::ios::out | std::ios::binary);
}

void movest_encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride) {
    algorithm->encode(mvs, mb_type, mb_width, mb_height, mv_stride);
}

void movest_decode(int16_t (*mvs[2])[2], int mv_sample_log2, int mb_width, int mb_height, int mv_stride) {
    algorithm->decode(mvs, mv_sample_log2, mb_width, mb_height, mv_stride);
}

void movest_init_algorithm(const char *algname) {
    if(algorithm != nullptr) {
        delete algorithm;
        algorithm = nullptr;
    }

    if(std::strcmp(algname, "hidenseek") == 0) {
        algorithm = new HideSeek();
    }
}

void movest_init_encoder(const char* filename) {
    algorithm->initAsEncoder(filename);
}

void movest_init_decoder(const char* filename) {
    algorithm->initAsDecoder(filename);
}

