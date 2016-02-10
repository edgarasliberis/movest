//
// Created by el on 08/02/16.
//

#include <iostream>
#include "CryptoFile.h"

CryptoFile::CryptoFile(const std::string &path, std::ios_base::openmode mode) {
    this->encrypt = false;
    stream.open(path, mode);
}

CryptoFile::CryptoFile(const std::string &path, uint8_t key[KeyLength], uint8_t iv[BlockSize], std::ios_base::openmode mode) {
    this->encrypt = true;
    stream.open(path, mode);
    std::copy(key, key + KeyLength, this->key);
    std::copy(iv, iv + BlockSize, this->iv);
    enc = std::make_shared<CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption>(this->key, sizeof(this->key), this->iv);
    dec = std::make_shared<CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption>(this->key, sizeof(this->key), this->iv);
}

std::streamsize CryptoFile::read(uint8_t *out, uint size) {
    if(stream.eof()) return 0;
    if(encrypt) {
        uint8_t buf[size];
        stream.read(reinterpret_cast<char*>(buf), size);
        enc->ProcessData(out, buf, (size_t)stream.gcount());
    } else {
        stream.read(reinterpret_cast<char*>(out), size); // Should never fail to read less, really
    }
    return stream.gcount();
}

std::streamsize CryptoFile::write(const uint8_t *out, uint size) {
    if(encrypt) {
        uint8_t buf[size];
        dec->ProcessData(buf, out, size);
        stream.write(reinterpret_cast<char*>(buf), size);
    } else {
        stream.write(reinterpret_cast<const char*>(out), size);
    }
    return stream.gcount();
}

void CryptoFile::flush() {
    stream.flush();
}

void CryptoFile::close() {
    stream.flush();
    stream.close();
}

bool CryptoFile::eof() {
    return stream.eof();
}

std::fstream& CryptoFile::exposeStream() {
    return stream;
}

uint CryptoFile::remainingData() {
    long begin = stream.tellg();
    stream.seekg(0, std::ios::end);
    long end = stream.tellg();
    stream.seekg(begin, std::ios::beg);

    return uint(end - begin);
}