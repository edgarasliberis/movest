#ifndef MOVEST_BUFFER_H
#define MOVEST_BUFFER_H

#include <algorithm>
#include <deque>
#include <cstddef>

template <typename T>
class Buffer {
public:
    void write(const T *array, unsigned long numElements) {
        data.insert(data.end(), array, array + numElements);
    }
    unsigned long read(T *out, unsigned long numElements) {
        if(numElements > length()) {
            numElements = length();
        }
        if(numElements == 0) return 0;

        std::copy_n(data.begin(), numElements, out);
        data.erase(data.begin(), data.begin()+numElements);
        return numElements;
    }
    unsigned long length() {
        return data.size();
    }
    bool empty() {
        return data.empty();
    }

private:
    std::deque<T> data;
};


#endif //MOVEST_BUFFER_H
