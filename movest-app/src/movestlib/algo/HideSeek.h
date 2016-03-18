//
// Created by el398 on 08/12/15.
//

#ifndef MOVEST_HIDESEEK_H
#define MOVEST_HIDESEEK_H

#include "../Algorithm.h"

class HideSeek : public Algorithm {
public:
    virtual void initAsDecoder(movest_params *params);
    virtual void initAsEncoder(movest_params *params);
    virtual void embedIntoMv(int16_t *mvX, int16_t *mvY);
    virtual void extractFromMv(int16_t mvX, int16_t mvY);

    virtual bool embedIntoMvComponent(int16_t *mv, int bit);
    virtual bool extractFromMvComponent(int16_t mv, int *bit);

    virtual movest_result finalise();

protected:
    void getDataToEmbed();
    void writeRecoveredData();
    virtual void processMvComponentEmbed(int16_t *mv);
    virtual void processMvComponentExtract(int16_t mv);

    bool stopEmbedding = false;
    uint indexLimit = 0;
    uint index = 0;
    uint8_t symb[CryptoFile::BlockSize];
};

#endif //MOVEST_HIDESEEK_H
