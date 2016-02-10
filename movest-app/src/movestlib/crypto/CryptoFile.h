//
// Created by el on 08/02/16.
//

#ifndef MOVEST_CRYPTOFILE_H
#define MOVEST_CRYPTOFILE_H

#include <fstream>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>

class CryptoFile {
public:
    static const int BlockSize = CryptoPP::AES::BLOCKSIZE;
    static const int KeyLength = CryptoPP::AES::MAX_KEYLENGTH;

    CryptoFile() {};
    CryptoFile(const std::string &path, std::ios_base::openmode mode);
    CryptoFile(const std::string &path, uint8_t key[KeyLength], uint8_t iv[BlockSize], std::ios::openmode mode);

    std::streamsize read(uint8_t *out, uint size);
    std::streamsize write(const uint8_t *out, uint size); // Will modify buffer during encryption
    uint remainingData();
    void flush();
    void close();
    bool eof();

    std::fstream& exposeStream();
private:
    std::shared_ptr<CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption> enc;
    std::shared_ptr<CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption> dec;
    uint8_t key[KeyLength], iv[BlockSize];
    bool encrypt;
    std::fstream stream;
};

#endif //MOVEST_CRYPTOFILE_H
