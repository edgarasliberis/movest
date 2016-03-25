//
// Created by el on 08/02/16.
//

#include <iostream>
#include "CryptoFile.h"

const int CryptoFile::BlockSize;
const int CryptoFile::KeyLength;

CryptoFile::CryptoFile(const std::string &path, std::ios_base::openmode mode): encrypt(false),
                                                                               fileStream(true) {
    this->stream = std::make_shared<std::fstream>(path, mode);
    if(this->stream->fail()) {
        std::cerr << "Can't open " << path << std::endl;
        exit(0);
    }
}

CryptoFile::CryptoFile(const std::string &path, uint8_t key[KeyLength], uint8_t iv[BlockSize],
                       std::ios_base::openmode mode): encrypt(true), fileStream(true) {
    this->stream = std::make_shared<std::fstream>(path, mode);
    if(this->stream->fail()) {
        std::cerr << "Can't open " << path << std::endl;
        exit(0);
    }
    initCrypto(key, iv);
}

std::streamsize CryptoFile::read(uint8_t *out, ulong size) {
    if(stream->eof()) return 0;
    if(encrypt) {
        uint8_t* buf = new uint8_t[size];
        stream->read(reinterpret_cast<char*>(buf), size);
        enc->ProcessData(out, buf, (size_t)stream->gcount());
        delete[] buf;
    } else {
        stream->read(reinterpret_cast<char*>(out), size); // Should never fail to read less, really
    }
    return stream->gcount();
}

std::streamsize CryptoFile::write(const uint8_t *out, ulong size) {
    if(encrypt) {
        uint8_t* buf = new uint8_t[size];
        dec->ProcessData(buf, out, size);
        stream->write(reinterpret_cast<char*>(buf), size);
        delete[] buf;
    } else {
        stream->write(reinterpret_cast<const char*>(out), size);
    }
    return stream->gcount();
}

void CryptoFile::flush() {
    stream->flush();
}

void CryptoFile::close() {
    stream->flush();
    if(fileStream) {
        std::iostream& fStream = this->exposeStream();
        dynamic_cast<std::fstream&>(fStream).close();
    }
}

bool CryptoFile::eof() {
    return stream->eof();
}

std::iostream& CryptoFile::exposeStream() {
    return *stream;
}

uint CryptoFile::remainingData() {
    long begin = stream->tellg();
    stream->seekg(0, std::ios::end);
    long end = stream->tellg();
    stream->seekg(begin, std::ios::beg);

    return uint(end - begin);
}

CryptoFile::CryptoFile(std::iostream *stream):
        encrypt(false), fileStream(false), stream(stream) {}

CryptoFile::CryptoFile(std::iostream *stream, uint8_t key[KeyLength], uint8_t iv[BlockSize]):
        encrypt(true), fileStream(false), stream(stream) {
    initCrypto(key, iv);
}

void CryptoFile::initCrypto(uint8_t key[KeyLength], uint8_t iv[BlockSize]) {
    this->enc = std::make_shared<CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption>(key, KeyLength, iv);
    this->dec = std::make_shared<CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption>(key, KeyLength, iv);
}
