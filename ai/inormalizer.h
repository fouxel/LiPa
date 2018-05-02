#ifndef INORMALIZER_H
#define INORMALIZER_H

#include "types.h"

class INormalizer
{
public:
    ~INormalizer() {}
    virtual distvec normalize(cdistvec &distances) = 0;

    virtual size_t maxValue() = 0;
};

#endif // INORMALIZER_H
