#ifndef NORMALIZER_H
#define NORMALIZER_H

#include "types.h"

class Normalizer
{
public:
    Normalizer();
    static distvec normalize(cdistvec &distances);
};

#endif // NORMALIZER_H
