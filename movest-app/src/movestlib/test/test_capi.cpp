#include <fstream>
#include "gtest/gtest.h"

#include "../movest_connector.h"


const char* algorithms[] = {
        "hidenseek",
        "rand-hidenseek",
        "dumpmvs", "dummypass",
        "msteg",
        "f3",
        "f4",
        "mvsteg",
        "outguess1",
        "xualg"
};

const int numAlgorithms = sizeof(algorithms) / sizeof(algorithms[0]);

/**
 * Test that initialisation as encoder doesn't fail.
 */
TEST(MovestAPI, InitLibraryAsEncoder) {
    uint32_t algoptions[2] = { 64, 32 };
    movest_params p = {
            "/dev/zero", MOVEST_NO_PARAMS, NULL
    };
    for(int i = 0; i < numAlgorithms; ++i) {
        movest_init_algorithm(algorithms[i], algoptions);
        movest_init_encoder(&p);
    }
}

/**
 * Test that initialisation as decoder doesn't fail.
 */
TEST(MovestAPI, InitLibraryAsDecoder) {
    movest_params p = {
            "/dev/zero", MOVEST_NO_PARAMS, NULL
    };
    for(int i = 0; i < numAlgorithms; ++i) {
        movest_init_algorithm(algorithms[i], NULL);
        movest_init_decoder(&p);
    }
}

/**
 * Full encoding path.
 */
TEST(MovestAPI, EncodeSeq) {
    // Will embed all zeros, but ModificationTest already tests every embedding algorithm, so it's important
    movest_params p = {
            "/dev/zero", MOVEST_NO_PARAMS, NULL
    };
    movest_init_algorithm("hidenseek", NULL);
    movest_init_encoder(&p);

    int width = 6, height = 3;
    int16_t mvs[][2] = { {0, 1}, {2, 3}, {4, 5}, {5, 5}, {2, 2}, {1, 0},
                         {0, 1}, {0, 0}, {4, 5}, {5, 5}, {2, 2}, {1, 0},
                         {0, 1}, {2, 3}, {4, 5}, {0, 0}, {2, 2}, {1, 0}};

    // 2 is a non-skip block; skip 2 MBs, leaving 32 bits of data (= 4 bytes)
    uint16_t type[] = { 2, 2, 2, 2, 2, 2,
                        2, 1, 2, 2, 2, 2,
                        2, 2, 2, 1, 2, 2 };
    movest_encode(mvs, type, width, height, width);

    int16_t exp[][2] = { {0, 0}, {2, 2}, {4, 4}, {4, 4}, {2, 2}, {0, 0},
                         {0, 0}, {0, 0}, {4, 4}, {4, 4}, {2, 2}, {0, 0},
                         {0, 0}, {2, 2}, {4, 4}, {0, 0}, {2, 2}, {0, 0}};

    for(int i = 0; i < width * height; ++i) {
        EXPECT_EQ(exp[i][0], mvs[i][0]);
        EXPECT_EQ(exp[i][1], mvs[i][1]);
    }

    movest_result result = movest_finalise();
    EXPECT_EQ(4, result.bytes_processed);
    EXPECT_EQ(1, result.error_code); // /dev/zero never depletes
}

/**
 * Full decoding path.
 */
TEST(MovestAPI, DecodeSeq) {
    char *file = strdup("/tmp/movest_testXXXXXX");
    close(mkstemp(file));

    movest_params p = {
            file, MOVEST_NO_PARAMS, NULL
    };
    movest_init_algorithm("hidenseek", NULL);
    movest_init_decoder(&p);

    int width = 6, height = 3;
    int16_t mvs[2][18][2] = {{ {0, 1}, {2, 3}, {4, 5}, {5, 5}, {2, 2}, {1, 0},
                               {0, 1}, {0, 0}, {4, 5}, {5, 5}, {2, 2}, {1, 0},
                               {0, 1}, {2, 3}, {4, 5}, {0, 0}, {2, 2}, {1, 0}}, {}};

    // 2 is a non-skip block; skip 2 MBs, leaving 32 bits of data (= 4 bytes)
    uint32_t type[] = { 2, 2, 2, 2, 2, 2,
                        2, 1, 2, 2, 2, 2,
                        2, 2, 2, 1, 2, 2 };
    int16_t* arr[2] = {mvs[0][0], mvs[1][0]};
    movest_decode((int16_t(**)[2])arr, type, 0, width, height, width, width);

    movest_result result = movest_finalise();
    EXPECT_EQ(4, result.bytes_processed);
    EXPECT_EQ(0, result.error_code); // /dev/zero is always eof

    unsigned char expected[4] = { 0xea, 0xa4, 0x93, 0x4a };
    unsigned char received[4];
    std::fstream fd(file, std::ios::in);

    fd.read((char*)received, 4);
    EXPECT_EQ(expected[0], received[0]);
    EXPECT_EQ(expected[1], received[1]);
    EXPECT_EQ(expected[2], received[2]);
    EXPECT_EQ(expected[3], received[3]);
}