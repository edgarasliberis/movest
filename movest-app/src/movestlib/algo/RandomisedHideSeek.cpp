//
// Created by el398 on 10/12/15.
//

#include "RandomisedHideSeek.h"
#include "../movest_connector.h"

#include <algorithm>
#include <iostream>
#include <assert.h>

extern "C" {
#include <rscode/ecc.h>
#define BLOCKSIZE 255
}

void RandomisedHideSeek::initAsEncoder(movest_params *params) {
    Algorithm::initAsEncoder(params);
    if(!(flags & MOVEST_DUMMY_PASS)) {
        initialize_ecc();

        // Determine the size of the file
        long begin = datafile.tellg();
        datafile.seekg(0, std::ios::end);
        long end = datafile.tellg();
        datafile.clear();
        datafile.seekg(0, std::ios::beg);

        fileSize = uint(end - begin);
        // Total size of embedded data:
        // fileSize + NPAR parity bytes for every (BLOCKSIZE - NPAR) bytes of the file
        uint blocks = (fileSize / (BLOCKSIZE - NPAR)) + (fileSize % (BLOCKSIZE - NPAR) != 0);
        dataSize = blocks * NPAR + fileSize;

        initialiseMapping(static_cast<AlgOptions*>(params->algParams), dataSize);

        // Fill the data buffer with blocks of file data & parity bytes
        data = new unsigned char[dataSize];
        unsigned char fileData[BLOCKSIZE - NPAR];
        uint currentPos = 0;
        while(!datafile.eof() && currentPos < dataSize) {
            datafile.read(reinterpret_cast<char*>(&fileData[0]), BLOCKSIZE - NPAR);
            encode_data(fileData, (int)datafile.gcount(), data + currentPos);
            currentPos += (int)datafile.gcount() + NPAR;
        }
    }
}

void RandomisedHideSeek::initAsDecoder(movest_params *params) {
    Algorithm::initAsDecoder(params);
    if(!(flags & MOVEST_DUMMY_PASS)) {
        initialize_ecc();
        AlgOptions *opt = static_cast<AlgOptions*>(params->algParams);
        fileSize = opt->fileSize;

        // Total size of embedded data:
        // fileSize + NPAR parity bytes for every (BLOCKSIZE - NPAR) bytes of the file
        uint blocks = (fileSize / (BLOCKSIZE - NPAR)) + (fileSize % (BLOCKSIZE - NPAR) != 0);
        dataSize = blocks * NPAR + fileSize;

        initialiseMapping(opt, dataSize);

        data = new unsigned char[dataSize]();
    }
}

void RandomisedHideSeek::initialiseMapping(AlgOptions *algParams, uint dataSize) {
    // Build a mapping from a data bit to the particular MV
    uint64_t capacity = algParams->byteCapacity;

    assert(encoder || dataSize <= capacity);

    std::vector<uint8_t> seedData = this->deriveBytes(SEED_SIZE, "MovestRandSeed");
    std::seed_seq seed(seedData.begin(), seedData.end());
    std::default_random_engine rng(seed);

    ulong bitCapacity = ((ulong) capacity) * 8;
    std::uniform_int_distribution<ulong> dist(0, bitCapacity);
    ulong bitDataSize = ((ulong) dataSize) * 8;
    bitToMvMapping = new Pair[bitDataSize];

    std::vector<bool> used(bitCapacity, false);

    for(ulong i = 0; i < bitDataSize; ++i) {
        ulong mvNum = dist(rng);
        while(used[mvNum]) {
            ++mvNum;
            if(mvNum >= bitCapacity) mvNum = 0;
        }
        used[mvNum] = true;
        bitToMvMapping[i] = Pair { i,  mvNum };
    }

    // Sort the mapping in increasing order of MVs, for sequential embedding
    std::sort(bitToMvMapping, bitToMvMapping + bitDataSize);
}

void RandomisedHideSeek::embedIntoMv(int16_t *mv) {
    if(flags & MOVEST_DUMMY_PASS) {
        if(true /* *mv != 0 && *mv != 1*/) {
            bitsProcessed++;
        }
    } else {
        if(index >= 8*dataSize) return;
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
    if(index >= 8*dataSize) return;
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
    if(!encoder && !(flags & MOVEST_DUMMY_PASS)) {
        uint currentPos = 0;
        while(currentPos < dataSize) {
            uint blockSize = std::min((uint)BLOCKSIZE, dataSize - currentPos);
            decode_data(data + currentPos, blockSize);
            if (check_syndrome() != 0) {
                correct_errors_erasures(data + currentPos, blockSize, 0, NULL);
            }
            datafile.write(reinterpret_cast<char*>(&data[0] + currentPos), blockSize - NPAR);
            currentPos += blockSize;
        }

        delete data;
        delete bitToMvMapping;
    }
    return Algorithm::finalise();
}
