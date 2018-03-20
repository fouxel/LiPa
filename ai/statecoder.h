#ifndef STATECODER_H
#define STATECODER_H

#include "types.h"

class StateCoder
{
public:
    StateCoder(int maxValue);
    static state encode(cdistvec &vec, int maxValue); //TODO: Change to non-static
private:
    int m_maxValue;
};

#endif // STATECODER_H
