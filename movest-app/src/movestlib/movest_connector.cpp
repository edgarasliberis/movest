#include <iostream>
#include "movest_connector.h"
#include "Algorithm.h"

#include "algo/HideSeek.h"
#include "algo/RandomisedHideSeek.h"
#include "algo/DumpMvs.h"
#include "algo/MSteg.h"
#include "algo/F3.h"
#include "algo/F4.h"
#include "algo/MVSteg.h"
#include "algo/OutGuess1.h"
#include "algo/XuAlg.h"

static Algorithm *algorithm;

void movest_encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride) {
    if(algorithm == nullptr) {
        std::cerr << "movest_encode error: initialise algorithm first (movest_init_encoder)."
                  << std::endl;
        return;
    }
    algorithm->encode(mvs, mb_type, mb_width, mb_height, mv_stride);
}

void movest_decode(int16_t (*mvs[2])[2], uint32_t *mbtype_table, int mv_sample_log2, int mb_width, int mb_height, int mv_stride,
                   int mb_stride) {
    if(algorithm == nullptr) {
        std::cerr << "movest_decode error: initialise algorithm first (movest_init_decoder)."
                  << std::endl;
        return;
    }
    algorithm->decode(mvs, mbtype_table, mv_sample_log2, mb_width, mb_height, mv_stride, mb_stride);
}

void init_algorithm(const char *algname, void* alg_params) {
    if(algorithm != nullptr) {
        delete algorithm;
    }

    if(std::strcmp(algname, "hidenseek") == 0) {
        algorithm = new HideSeek();
    }
    else if(std::strcmp(algname, "rand-hidenseek") == 0) {
        algorithm = new RandomisedHideSeek(reinterpret_cast<RandomisedHideSeek::AlgOptions*>(alg_params));
    }
    else if(std::strcmp(algname, "dumpmvs") == 0 || std::strcmp(algname, "dummypass") == 0) {
        algorithm = new DumpMvs();
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
        algorithm = new OutGuess1(reinterpret_cast<RandomisedHideSeek::AlgOptions*>(alg_params));
    }
    else if(std::strcmp(algname, "xualg") == 0) {
        algorithm = new XuAlg();
    }
}

void movest_init_encoder(const char *algname, movest_params *params, void* alg_params) {
    init_algorithm(algname, alg_params);
    algorithm->initAsEncoder(params);
}

void movest_init_decoder(const char *algname, movest_params *params, void* alg_params) {
    init_algorithm(algname, alg_params);
    algorithm->initAsDecoder(params);
}

unsigned int movest_get_embedded_data_size(unsigned int data_size) {
    if(algorithm == nullptr) {
        std::cerr << "movest_get_embedded_data_size error: initialise algorithm first (movest_init_encoder)."
                  << std::endl;
        exit(1);
    }
    return algorithm->computeEmbeddingSize(data_size);
}

movest_result movest_finalise() {
    movest_result result = algorithm->finalise();
    delete algorithm;
    algorithm = nullptr;
    return result;
}
