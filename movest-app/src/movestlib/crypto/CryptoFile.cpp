//
// Created by el on 08/02/16.
//

#include <iostream>
#include "CryptoFile.h"

const int CryptoFile::BlockSize;
const int CryptoFile::KeyLength;

CryptoFile::CryptoFile(const std::string &path, std::ios_base::openmode mode): encrypt(false) {
    this->stream = std::make_shared<std::fstream>(path, mode);
    if(this->stream->fail()) {
        std::cerr << "Can't open " << path << std::endl;
        exit(1);
    }
    this->opened = true;
}

CryptoFile::CryptoFile(const std::string &path, uint8_t key[KeyLength],
                       std::ios_base::openmode mode): encrypt(true) {
    this->stream = std::make_shared<std::fstream>(path, mode);
    if(this->stream->fail()) {
        std::cerr << "Can't open " << path << std::endl;
        exit(0);
    }
    this->opened = true;
    std::copy(key, key + KeyLength, this->key);
}

std::streamsize CryptoFile::read(uint8_t *out, ulong size) {
    if(size == 0) return 0;

    // Not enough data available to satisfy the request
    if(buffer.length() < size) {
        ulong numBytes = size - buffer.length();

        // Populate the buffer
        uint8_t* buf = new uint8_t[numBytes];
        stream->read(reinterpret_cast<char*>(buf), size);
        size_t numRead = (size_t)stream->gcount();
        if(encrypt) {
            enc->ProcessData(buf, buf, numRead);
        }
        buffer.write(buf, numRead);
        delete[] buf;
    }

    return buffer.read(out, size);
}

void CryptoFile::setIv(uint8_t iv[BlockSize]) {
    std::copy(iv, iv + BlockSize, this->iv);
    initCrypto();
    this->hasIv = true;
    buffer.write(this->iv, (unsigned)BlockSize);
}

std::streamsize CryptoFile::write(const uint8_t *out, ulong size) {
    buffer.write(out, size);

    // Capture IV
    if(encrypt && !hasIv && buffer.length() >= CryptoFile::BlockSize) {
        buffer.read(this->iv, (unsigned) CryptoFile::BlockSize);
        this->hasIv = true;
        initCrypto();
    }

    this->writeOutBuffer();

    return size;
}

void CryptoFile::writeOutBuffer() {
    ulong size = buffer.length();
    if(size == 0 || (encrypt && !hasIv)) return;

    uint8_t *buf = new uint8_t[size];
    buffer.read(buf, size);
    if(encrypt) {
        dec->ProcessData(buf, buf, size);
    }
    stream->write(reinterpret_cast<char*>(buf), size);
    delete[] buf;
}

void CryptoFile::flush() {
    if(!opened) return;
    stream->flush();
}

void CryptoFile::close() {
    this->flush();
}

bool CryptoFile::eof() {
    if(!opened) return true;
    return stream->eof() && buffer.length() == 0;
}

std::iostream& CryptoFile::exposeStream() {
    return *stream;
}

ulong CryptoFile::remainingData() {
    long begin = stream->tellg();
    stream->seekg(0, std::ios::end);
    long end = stream->tellg();
    stream->seekg(begin, std::ios::beg);

    return end - begin + buffer.length();
}

CryptoFile::CryptoFile(std::iostream *stream):
        encrypt(false), opened(true), stream(stream) {}

CryptoFile::CryptoFile(std::iostream *stream, uint8_t key[KeyLength]):
        encrypt(true), opened(true), stream(stream) {
    std::copy(key, key + KeyLength, this->key);
}

void CryptoFile::initCrypto() {
    this->enc = std::make_shared<CryptoPP::CTR_Mode<CryptoPP::AES>::Encryption>(key, KeyLength, iv);
    this->dec = std::make_shared<CryptoPP::CTR_Mode<CryptoPP::AES>::Decryption>(key, KeyLength, iv);
}

unsigned int CryptoFile::encryptedFileSize(unsigned int initialSize, bool encryptionEnabled) {
    return encryptionEnabled? (initialSize + CryptoFile::BlockSize) : initialSize;
}
