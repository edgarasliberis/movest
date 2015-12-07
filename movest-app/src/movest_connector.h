//
// Created by el398 on 06/12/15.
//

#ifndef MOVEST_CONNECTOR_H
#define MOVEST_CONNECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

struct MovestState {
    const char *file;
};

void movest_encode(int width, int height);
void movest_decode(int width, int height);

#ifdef __cplusplus
}
#endif

#endif //MOVEST_CONNECTOR_H
