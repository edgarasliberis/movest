//
// Created by el398 on 07/12/15.
//

#include <iostream>
#include <cstring>

#include "algorithm.h"
#include "algo/HideSeek.h"
#include "algo/RandomisedHideSeek.h"
#include "algo/Dummy.h"
#include "algo/MSteg.h"
#include "algo/F3.h"
#include "algo/F4.h"
#include "algo/MVSteg.h"
#include "algo/OutGuess1.h"
#include "algo/MVStegVuln.h"

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
    int error = 0;

    if(encoder && !datafile.eof()) {
        error = 1;
    }
    datafile.close();
    return movest_result {
            uint(bitsProcessed / 8), error
    };
}

void movest_encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride) {
    algorithm->encode(mvs, mb_type, mb_width, mb_height, mv_stride);
}

void movest_decode(int16_t (*mvs[2])[2], uint32_t *mbtype_table, int mv_sample_log2, int mb_width, int mb_height, int mv_stride,
                   int mb_stride) {
    algorithm->decode(mvs, mbtype_table, mv_sample_log2, mb_width, mb_height, mv_stride, mb_stride);
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
    else if(std::strcmp(algname, "dumpmvs") == 0 || std::strcmp(algname, "dummypass") == 0) {
        algorithm = new Dummy();
    }
    else if(std::strcmp(algname, "msteg") == 0) {
        algorithm = new MSteg();
    }
    else if(std::strcmp(algname, "f3") == 0) {
        algorithm = new F3();
    }
    else if(std::strcmp(algname, "f4") == 0) {
        algorithm = new F4();
    }
    else if(std::strcmp(algname, "mvsteg") == 0) {
        algorithm = new MVSteg();
    }
    else if(std::strcmp(algname, "outguess1") == 0) {
        algorithm = new OutGuess1();
    }
    else if(std::strcmp(algname, "mvsteg-vuln") == 0) {
        algorithm = new MVStegVuln();
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
