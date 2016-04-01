#include <algorithm>

#include "gtest/gtest.h"
#include "../crypto/CryptoFile.h"

/**
 * Verify that reads do not perform encryption.
 */
TEST(CryptoFileTest, ReadNoEncryption)
{
    std::string input("This is the input test string.");
    uint8_t *buff = new uint8_t[input.length()];

    // CryptoFile will take ownership
    std::stringstream *stream = new std::stringstream(input);
    CryptoFile cfile(stream);
    cfile.read(buff, input.length());

    auto actual = std::string(reinterpret_cast<char*>(buff), input.size());
    EXPECT_EQ(input, actual);
    delete[] buff;
}

/**
 * Verify that writes do not perform decryption.
 */
TEST(CryptoFileTest, WriteNoDecryption)
{
    std::string input("This is the input test string.");
    const uint8_t *inputBuffer = reinterpret_cast<const uint8_t*>(input.c_str());

    // CryptoFile will take ownership
    std::stringstream *stream = new std::stringstream();
    CryptoFile cfile(stream);
    cfile.write(inputBuffer, input.length());

    EXPECT_EQ(input, stream->str());
}

// F.5.5 CTR-AES256.Encrypt
uint8_t key[CryptoFile::KeyLength] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
                                       0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                                       0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
                                       0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };
uint8_t iv[CryptoFile::BlockSize] = { 0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
                                      0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff };

uint8_t plaintext[] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
                       0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};
uint8_t ciphertext[] = {0x60, 0x1e, 0xc3, 0x13, 0x77, 0x57, 0x89, 0xa5,
                        0xb7, 0xa7, 0xf5, 0x04, 0xbb, 0xf3, 0xd2, 0x28};

// RFC 3686
/*uint8_t key[CryptoFile::KeyLength] = { 0x77, 0x6B, 0xEF, 0xF2, 0x85, 0x1D, 0xB0, 0x6F,
                                       0x4C, 0x8A, 0x05, 0x42, 0xC8, 0x69, 0x6F, 0x6C,
                                       0x6A, 0x81, 0xAF, 0x1E, 0xEC, 0x96, 0xB4, 0xD3,
                                       0x7F, 0xC1, 0xD6, 0x89, 0xE6, 0xC1, 0xC1, 0x04 };
uint8_t plaintext[] = {0x53, 0x69, 0x6E, 0x67, 0x6C, 0x65, 0x20, 0x62,
                       0x6C, 0x6F, 0x63, 0x6B, 0x20, 0x6D, 0x73, 0x67 };
uint8_t iv[CryptoFile::BlockSize] = { 0x00, 0x00, 0x00, 0x60, 0xDB, 0x56, 0x72, 0xC9,
                                      0x7A, 0xA8, 0xF0, 0xB2, 0x00, 0x00, 0x00, 0x01 };
uint8_t ciphertext[] = {0x14, 0x5A, 0xD0, 0x1D, 0xBF, 0x82, 0x4E, 0xC7,
                        0x56, 0x08, 0x63, 0xDC, 0x71, 0xE3, 0xE0, 0xC0 };*/

/**
 * Verify that reads do perform encryption.
 */
TEST(CryptoFileTest, ReadAESCTREncryption)
{
    // CryptoFile will take ownership
    std::stringstream *stream = new std::stringstream(std::string(reinterpret_cast<char*>(plaintext)));
    uint8_t buff[sizeof(ciphertext)];
    CryptoFile cfile(stream, key, iv);
    cfile.read(buff, sizeof(buff));

    for(uint i = 0; i < sizeof(buff); i++)
        EXPECT_EQ(ciphertext[i], buff[i]);
}

/**
 * Verify that writes do perform decryption.
 */
TEST(CryptoFileTest, WriteAESCTRDecryption)
{
    // CryptoFile will take ownership
    std::stringstream *stream = new std::stringstream();
    CryptoFile cfile(stream, key, iv);
    cfile.write(ciphertext, sizeof(ciphertext));
    std::string written = stream->str();

    for(uint i = 0; i < sizeof(plaintext); i++)
        EXPECT_EQ(plaintext[i], (uint8_t)written[i]);
}
