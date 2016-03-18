#include "gtest/gtest.h"

#include "../algo/HideSeek.h"
#include "../algo/MSteg.h"
#include "../algo/F3.h"
#include "../algo/F4.h"
#include "../algo/MVSteg.h"
#include "../algo/XuAlg.h"

TEST(ModificationTest, HideAndSeekEmbed)
{
    auto algo = HideSeek();
    int16_t mvs[] = { 0, 0, 1, 1, -1, -1, -2, -2};
    int bits[]    = { 0, 1, 0, 1,  0,  1,  0,  1};
    int result[]  = { 0, 1, 0, 1, -2, -1, -2, -1};

    for(uint i = 0; i < sizeof(mvs) / sizeof(mvs[0]); ++i) {
        EXPECT_TRUE(algo.embedIntoMvComponent(&mvs[i], bits[i]));
        EXPECT_EQ(result[i], mvs[i]);
    }
}

TEST(ModificationTest, HideAndSeekExtract)
{
    auto algo = HideSeek();
    int16_t mvs[] = { 0, 0, 1, 1, -1, -1, -2, -2};
    int bits[]    = { 0, 1, 0, 1,  0,  1,  0,  1};
    int result[]  = { 0, 1, 0, 1, -2, -1, -2, -1};

    for(uint i = 0; i < sizeof(mvs) / sizeof(mvs[0]); ++i) {
        int bit = 0;
        EXPECT_TRUE(algo.extractFromMvComponent(result[i], &bit));
        EXPECT_EQ(bits[i], bit & 1);
    }
}

TEST(ModificationTest, MStegEmbed)
{
    auto algo = MSteg();
    int16_t mvs[]  = { 0, 0, 1, 1, -1, -1, -2, -2, 2, 2, 3, 3};
    int bits[]     = { 0, 1, 0, 1,  0,  1,  0,  1, 0, 1, 0, 1};
    int result[]   = { 0, 0, 1, 1, -2, -1, -2, -1, 2, 3, 2, 3};
    bool success[] = { 0, 0, 0, 0,  1,  1,  1,  1, 1, 1, 1, 1};

    for(uint i = 0; i < sizeof(mvs) / sizeof(mvs[0]); ++i) {
        EXPECT_EQ(success[i], algo.embedIntoMvComponent(&mvs[i], bits[i]));
        EXPECT_EQ(result[i], mvs[i]);
    }
}

TEST(ModificationTest, MStegExtract)
{
    auto algo = MSteg();
    int16_t mvs[]  = { 0, 0, 1, 1, -1, -1, -2, -2, 2, 2, 3, 3};
    int bits[]     = { 0, 0, 0, 0,  0,  1,  0,  1, 0, 1, 0, 1};
    int result[]   = { 0, 0, 1, 1, -2, -1, -2, -1, 2, 3, 2, 3};
    bool success[] = { 0, 0, 0, 0,  1,  1,  1,  1, 1, 1, 1, 1};

    for(uint i = 0; i < sizeof(mvs) / sizeof(mvs[0]); ++i) {
        int bit = 0;
        EXPECT_EQ(success[i], algo.extractFromMvComponent(result[i], &bit));
        EXPECT_EQ(bits[i], bit & 1);
    }
}

TEST(ModificationTest, F3Embed)
{
    auto algo = F3();
    int16_t mvs[]  = { 0, 0, 1, 1, -1, -1, -2, -2, 2, 2 };
    int bits[]     = { 0, 1, 0, 1,  0,  1,  0,  1, 0, 1 };
    int result[]   = { 0, 0, 0, 1,  0, -1, -2, -1, 2, 1 };
    bool success[] = { 0, 0, 0, 1,  0,  1,  1,  1, 1, 1 };

    for(uint i = 0; i < sizeof(mvs) / sizeof(mvs[0]); ++i) {
        EXPECT_EQ(success[i], algo.embedIntoMvComponent(&mvs[i], bits[i]));
        EXPECT_EQ(result[i], mvs[i]);
    }
}

TEST(ModificationTest, F3Extract)
{
    auto algo = F3();
    int16_t mvs[]  = { 0, 0, 1, 1, -1, -1, -2, -2, 2, 2 };
    int bits[]     = { 0, 0, 0, 1,  0,  1,  0,  1, 0, 1 };
    int result[]   = { 0, 0, 0, 1,  0, -1, -2, -1, 2, 1 };
    bool success[] = { 0, 0, 0, 1,  0,  1,  1,  1, 1, 1 };

    for(uint i = 0; i < sizeof(mvs) / sizeof(mvs[0]); ++i) {
        int bit = 0;
        EXPECT_EQ(success[i], algo.extractFromMvComponent(result[i], &bit));
        EXPECT_EQ(bits[i], bit & 1);
    }
}

TEST(ModificationTest, F4Embed)
{
    auto algo = F4();
    int16_t mvs[]  = { 0, 0, 1, 1, -1, -1, -2, -2, 2, 2 };
    int bits[]     = { 0, 1, 0, 1,  0,  1,  0,  1, 0, 1 };
    int result[]   = { 0, 0, 0, 1, -1,  0, -1, -2, 2, 1 };
    bool success[] = { 0, 0, 0, 1,  1,  0,  1,  1, 1, 1 };

    for(uint i = 0; i < sizeof(mvs) / sizeof(mvs[0]); ++i) {
        EXPECT_EQ(success[i], algo.embedIntoMvComponent(&mvs[i], bits[i]));
        EXPECT_EQ(result[i], mvs[i]);
    }
}

TEST(ModificationTest, F4Extract)
{
    auto algo = F4();
    int16_t mvs[]  = { 0, 0, 1, 1, -1, -1, -2, -2, 2, 2 };
    int bits[]     = { 0, 0, 0, 1,  0,  0,  0,  1, 0, 1 };
    int result[]   = { 0, 0, 0, 1, -1,  0, -1, -2, 2, 1 };
    bool success[] = { 0, 0, 0, 1,  1,  0,  1,  1, 1, 1 };

    for(uint i = 0; i < sizeof(mvs) / sizeof(mvs[0]); ++i) {
        int bit = 0;
        EXPECT_EQ(success[i], algo.extractFromMvComponent(result[i], &bit));
        EXPECT_EQ(bits[i], bit & 1);
    }
}

TEST(ModificationTest, MVStegEmbed)
{
    auto algo = MVSteg();
    int16_t mvx[]  = { 0, 10,  0, -10,   0, 11,  0,   0, -11, 10, 10, 11, 11 };
    int16_t mvy[]  = { 0,  0, 10,   0, -10,  0, 11, -11,   0, 10, 10, 11, 11 };
    int bits[]     = { 0,  1,  0,   1,   0,  1,  0,   0,   1,  1,  0,  1,  0 };
    int resultx[]  = { 0,  9,  0, -10,   0, 11,  0,   0, -10,  9, 10, 11, 10 };
    int resulty[]  = { 0,  0, 10,   0,  -9,  0, 10, -11,   0,  9, 10, 11, 10 };
    bool success[] = { 0,  0,  1,   1,   0,  1,  1,   1,   1,  1,  1,  1,  1 };

    for(uint i = 0; i < sizeof(mvx) / sizeof(mvx[0]); ++i) {
        EXPECT_EQ(success[i], algo.doEmbedding(&mvx[i], &mvy[i], bits[i]));
        EXPECT_EQ(resultx[i], mvx[i]);
        EXPECT_EQ(resulty[i], mvy[i]);
    }
}

TEST(ModificationTest, MVStegExtract)
{
    auto algo = MVSteg();
    int16_t mvx[]  = { 0, 10,  0, -10,   0, 11,  0,   0, -11, 10, 10, 11, 11 };
    int16_t mvy[]  = { 0,  0, 10,   0, -10,  0, 11, -11,   0, 10, 10, 11, 11 };
    int bits[]     = { 0,  0,  0,   1,   0,  1,  0,   0,   1,  1,  0,  1,  0 };
    int resultx[]  = { 0,  9,  0, -10,   0, 11,  0,   0, -10,  9, 10, 11, 10 };
    int resulty[]  = { 0,  0, 10,   0,  -9,  0, 10, -11,   0,  9, 10, 11, 10 };
    bool success[] = { 0,  0,  1,   1,   0,  1,  1,   1,   1,  1,  1,  1,  1 };

    for(uint i = 0; i < sizeof(mvx) / sizeof(mvx[0]); ++i) {
        int bit = 0;
        EXPECT_EQ(success[i], algo.doExtraction(resultx[i], resulty[i], &bit));
        EXPECT_EQ(bits[i], bit & 1);
    }
}

TEST(ModificationTest, XuAlgEmbed)
{
    auto algo = XuAlg();
    int16_t mvx[]  = { 0,  0,  0,  9, 9, -5, -5,  -5, -5,  5,  5, 30, 30,  -1,  -1 };
    int16_t mvy[]  = { 0, 10, 10,  5, 5,  9,  9,  -9, -9, -9, -9,  1,  1, -30, -30 };
    int bits[]     = { 0,  0,  1,  0, 1,  0,  1,   0,  1,  0,  1,  0,  1,   0,   1 };
    int resultx[]  = { 0,  0,  0, 10, 9, -5, -5,  -5, -5,  6,  5, 30, 31,  -1,  -1 };
    int resulty[]  = { 0, 10, 11,  5, 5, 10,  9, -10, -9, -9, -9,  1,  1, -30, -31 };
    bool success[] = { 0,  1,  1,  1, 1,  1,  1,   1,  1,  1,  1,  1,  0,   1,   0 };

    for(uint i = 0; i < sizeof(mvx) / sizeof(mvx[0]); ++i) {
        EXPECT_EQ(success[i], algo.doEmbedding(&mvx[i], &mvy[i], bits[i]));
        EXPECT_EQ(resultx[i], mvx[i]);
        EXPECT_EQ(resulty[i], mvy[i]);
    }
}

TEST(ModificationTest, XuAlgExtract)
{
    auto algo = XuAlg();
    int16_t mvx[]  = { 0,  0,  0,  9, 9, -5, -5,  -5, -5,  5,  5, 30, 30,  -1,  -1 };
    int16_t mvy[]  = { 0, 10, 10,  5, 5,  9,  9,  -9, -9, -9, -9,  1,  1, -30, -30 };
    int bits[]     = { 0,  0,  1,  0, 1,  0,  1,   0,  1,  0,  1,  0,  0,   0,   0 };
    int resultx[]  = { 0,  0,  0, 10, 9, -5, -5,  -5, -5,  6,  5, 30, 31,  -1,  -1 };
    int resulty[]  = { 0, 10, 11,  5, 5, 10,  9, -10, -9, -9, -9,  1,  1, -30, -31 };
    bool success[] = { 0,  1,  1,  1, 1,  1,  1,   1,  1,  1,  1,  1,  0,   1,   0 };

    for(uint i = 0; i < sizeof(mvx) / sizeof(mvx[0]); ++i) {
        int bit = 0;
        EXPECT_EQ(success[i], algo.doExtraction(resultx[i], resulty[i], &bit));
        EXPECT_EQ(bits[i], bit & 1);
    }
}