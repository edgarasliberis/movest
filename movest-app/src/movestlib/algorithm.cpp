//
// Created by el398 on 07/12/15.
//

#include <iostream>
#include <cstring>

#include "algorithm.h"
#include "algo/HideSeek.h"
#include "algo/RandomisedHideSeek.h"

void Algorithm::initAsEncoder(movest_params *params) {
    datafile.open(params->filename, std::ios::in | std::ios::binary);
    flags = params->flags;
    encoder = true;
}

void Algorithm::initAsDecoder(movest_params *params) {
    datafile.open(params->filename, std::ios::out | std::ios::binary);
    flags = params->flags;
    encoder = false;
}

movest_result Algorithm::finalise() {
    datafile.close();
    return movest_result {
            uint(bitsProcessed / 8),
            0, NULL
    };
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
    }

    if(std::strcmp(algname, "hidenseek") == 0) {
        algorithm = new HideSeek();
    }
    else if(std::strcmp(algname, "rand-hidenseek") == 0) {
        algorithm = new RandomisedHideSeek();
    }
}

void movest_init_encoder(movest_params *params) {
    algorithm->initAsEncoder(params);
}

void movest_init_decoder(movest_params *params) {
    algorithm->initAsDecoder(params);
}

movest_result movest_finalise() {
    movest_result result = algorithm->finalise();
    delete algorithm;
    algorithm = nullptr;
    return result;
}
