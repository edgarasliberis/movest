//
// Created by el398 on 07/12/15.
//

#include <iostream>
#include <cstring>
#include <cryptopp/sha.h>
#include <cryptopp/pwdbased.h>

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
#include "movest_connector.h"

#define CRYPTO_SALT "MovestEncryption"
#define DEFAULT_PASSWORD "MovestDefaultPassword"

void Algorithm::initAsEncoder(movest_params *params) {
    this->encoder = true;
    this->initialiseAlgorithm(params);
}

void Algorithm::initAsDecoder(movest_params *params) {
    this->encoder = false;
    this->initialiseAlgorithm(params);
}

void Algorithm::initialiseAlgorithm(movest_params *params) {
    // Unpack parameters
    this->flags = params->flags;
    this->password = params->password == nullptr? DEFAULT_PASSWORD : params->password;
    auto iosFlags = std::ios::binary | ((this->encoder)? std::ios::in : std::ios::out);

    // Set up encryption
    if(this->flags & MOVEST_ENABLE_ENCRYPTION) {
        std::vector<uint8_t> bytes = deriveBytes(CryptoFile::KeyLength + CryptoFile::BlockSize, CRYPTO_SALT);
        datafile = CryptoFile(params->filename, &bytes[0], &bytes[CryptoFile::KeyLength], iosFlags);
    } else {
        datafile = CryptoFile(params->filename, iosFlags);
    }
}

void Algorithm::encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride) {
    for (int mb_y = 0; mb_y < mb_height; ++mb_y) {
        for (int mb_x = 0; mb_x < mb_width; ++mb_x) {
            int xy = mb_y * mv_stride + mb_x;
            if (mb_type[xy] == 2) {
                embedToPair(&mvs[xy][0], &mvs[xy][1]);
            }
        }
    }
}

void Algorithm::decode(int16_t (*mvs[2])[2], uint32_t *mbtype_table, int mv_sample_log2, int mb_width, int mb_height,
                      int mv_stride, int mb_stride) {
    for (int mb_y = 0; mb_y < mb_height; mb_y++) {
        for (int mb_x = 0; mb_x < mb_width; mb_x++) {
            int xy = (mb_x + mb_y * mv_stride) << mv_sample_log2;
            if(mbtype_table[mb_x + mb_y * mb_stride] != 1) {
                extractFromPair(mvs[0][xy][0], mvs[0][xy][1]);
            }
        }
    }
}

std::vector<uint8_t> Algorithm::deriveBytes(size_t numBytes, std::string salt) {
    std::vector<byte> derived = std::vector<byte>(numBytes);
    const byte *passwordPtr = reinterpret_cast<const byte*>(this->password.c_str());
    const byte *saltPtr = reinterpret_cast<const byte*>(salt.c_str());

    CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA1> pbkdf2;
    pbkdf2.DeriveKey(&derived[0], numBytes, 0, passwordPtr, this->password.length(), saltPtr, salt.length(), 100000);

    return derived;
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
