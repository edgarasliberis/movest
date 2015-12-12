//
// Created by el398 on 10/12/15.
//

#include "RandomisedHideSeek.h"

#include <algorithm>
#include <iostream>

RandomisedHideSeek::RandomisedHideSeek() {}

void RandomisedHideSeek::initAsEncoder(movest_params *params) {
    Algorithm::initAsEncoder(params);
    if(!(flags & MOVEST_DUMMY_PASS)) {
        // Read the whole file into the buffer
        long begin = datafile.tellg();
        datafile.seekg(0, std::ios::end);
        long end = datafile.tellg();
        datafile.clear();
        datafile.seekg(0, std::ios::beg);

        fileSize = uint(end - begin);
        data = new char[fileSize];
        datafile.read(data, fileSize);
        initialiseMapping(params, fileSize);
    }
}

void RandomisedHideSeek::initAsDecoder(movest_params *params) {
    Algorithm::initAsDecoder(params);
    if(!(flags & MOVEST_DUMMY_PASS)) {
        fileSize = static_cast<uint*>(params->algParams)[2];
        data = new char[fileSize]();
        initialiseMapping(params, fileSize);
    }
}

void RandomisedHideSeek::initialiseMapping(const movest_params *params, int fileSize) {
    // Build a mapping from a data bit to the particular MV
    uint seed = static_cast<uint*>(params->algParams)[0];
    uint capacity = static_cast<uint*>(params->algParams)[1];
    std::default_random_engine rng(seed);
    std::uniform_int_distribution<ulong> dist(0, (ulong) capacity * 8);
    bitToMvMapping = new Pair[8 * fileSize];

    for(ulong i = 0; i < 8 * fileSize; ++i) {
        bitToMvMapping[i] = Pair { i, dist(rng) };
    }

    // Sort the mapping in increasing order of MVs, for sequential embedding
    std::sort(bitToMvMapping, bitToMvMapping + 8*fileSize);
}

void RandomisedHideSeek::encode(int16_t (*mvs)[2], uint16_t *mb_type, int mb_width, int mb_height, int mv_stride) {
    for (int mb_y = 1; mb_y < mb_height; ++mb_y) {
        for (int mb_x = 1; mb_x < mb_width; ++mb_x) {
            int xy = mb_y * mv_stride + mb_x;
            if (mb_type[xy] != 1) {
                for(int comp = 0; comp < 2; ++comp) {
                    embedIntoMv(&mvs[xy][comp]);
                }
            }
        }
    }
}

void RandomisedHideSeek::embedIntoMv(int16_t *mv) {
    if(flags & MOVEST_DUMMY_PASS) {
        if(*mv != 0 && *mv != 1) {
            bitsProcessed++;
        }
    } else {
        if(index >= 8*fileSize) return;
        if(*mv != 0 && *mv != 1) {
            if (bitsProcessed == bitToMvMapping[index].s) {
                // We found a MV that's next on a list to be modified.
                ulong dataBit = bitToMvMapping[index].f;
                int bit = data[dataBit / 8] >> (dataBit % 8);

                if((bit & 1) && !(*mv & 1)) (*mv)++;
                if(!(bit & 1) && (*mv & 1)) (*mv)--;

                index++;
            }
            bitsProcessed++;
            std::cout << *mv << std::endl;
        }
    }
}

void RandomisedHideSeek::decode(int16_t (*mvs[2])[2], int mv_sample_log2, int mb_width, int mb_height, int mv_stride){
    for (int mb_y = 1; mb_y < mb_height; mb_y++) {
        for (int mb_x = 1; mb_x < mb_width; mb_x++) {
            int xy = (mb_x + mb_y * mv_stride) << mv_sample_log2;
            for(int comp = 0; comp < 2; ++comp) {
                extractFromMv(mvs[0][xy][comp]);
            }
        }
    }
}

void RandomisedHideSeek::extractFromMv(int16_t val) {
    if(index >= 8*fileSize) return;
    if (val != 0 && val != 1) {
        if (bitsProcessed == bitToMvMapping[index].s) {
            // We found a MV that was next on a list to be modified.
            ulong dataBit = bitToMvMapping[index].f;
            data[dataBit / 8] |= (val & 1) << (dataBit % 8);
            index++;
        }

        bitsProcessed++;
        std::cout << val << std::endl;
    }
}

movest_result RandomisedHideSeek::finalise() {
    if(!encoder) {
        datafile.write(data, fileSize);
        delete data;
        delete bitToMvMapping;
    }
    return Algorithm::finalise();
}
