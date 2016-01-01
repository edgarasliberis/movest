//
// Created by el398 on 10/12/15.
//

#include "RandomisedHideSeek.h"

#include <algorithm>
#include <iostream>
#include <assert.h>

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

        initialiseMapping(params, fileSize);
        data = new char[fileSize];
        datafile.read(data, fileSize);
    }
}

void RandomisedHideSeek::initAsDecoder(movest_params *params) {
    Algorithm::initAsDecoder(params);
    if(!(flags & MOVEST_DUMMY_PASS)) {
        fileSize = static_cast<uint*>(params->algParams)[2];
        initialiseMapping(params, fileSize);
        data = new char[fileSize]();
    }
}

void RandomisedHideSeek::initialiseMapping(const movest_params *params, uint fileSize) {
    // Build a mapping from a data bit to the particular MV
    uint seed = static_cast<uint*>(params->algParams)[0];
    uint capacity = static_cast<uint*>(params->algParams)[1];

    assert(encoder || fileSize <= capacity);

    std::default_random_engine rng(seed);
    ulong bitCapacity = ((ulong) capacity) * 8;
    std::uniform_int_distribution<ulong> dist(0, bitCapacity);
    ulong bitFileSize = ((ulong) fileSize) * 8;
    bitToMvMapping = new Pair[bitFileSize];

    std::vector<bool> used(bitCapacity, false);

    for(ulong i = 0; i < bitFileSize; ++i) {
        ulong mvNum = dist(rng);
        while(used[mvNum]) {
            ++mvNum;
            if(mvNum >= bitCapacity) mvNum = 0;
        }
        used[mvNum] = true;
        bitToMvMapping[i] = Pair { i,  mvNum };
    }

    // Sort the mapping in increasing order of MVs, for sequential embedding
    std::sort(bitToMvMapping, bitToMvMapping + bitFileSize);
}

void RandomisedHideSeek::embedIntoMv(int16_t *mv) {
    if(flags & MOVEST_DUMMY_PASS) {
        if(true /* *mv != 0 && *mv != 1*/) {
            bitsProcessed++;
        }
    } else {
        if(index >= 8*fileSize) return;
        if(true /* *mv != 0 && *mv != 1*/) {
            if (bitsProcessed == bitToMvMapping[index].mv) {
                // We found a MV that's next on a list to be modified.
                ulong dataBit = bitToMvMapping[index].bit;
                int bit = data[dataBit / 8] >> (dataBit % 8);

                if((bit & 1) && !(*mv & 1)) (*mv)++;
                if(!(bit & 1) && (*mv & 1)) (*mv)--;

                index++;
            }
        }
        bitsProcessed++;
    }
}

void RandomisedHideSeek::extractFromMv(int16_t val) {
    if(index >= 8*fileSize) return;
    if (true/* val != 0 && val != 1*/) {
        if (bitsProcessed == bitToMvMapping[index].mv) {
            // We found a MV that was next on a list to be modified.
            ulong dataBit = bitToMvMapping[index].bit;
            data[dataBit / 8] |= (val & 1) << (dataBit % 8);
            index++;
        }

        bitsProcessed++;
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
