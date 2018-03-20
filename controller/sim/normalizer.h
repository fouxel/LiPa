#ifndef NORMALIZER_H
#define NORMALIZER_H

#include "inormalizer.h"

class Normalizer : public INormalizer
{
public:
    Normalizer();
    distvec normalize(cdistvec &distances) override;
};

#endif // NORMALIZER_H
