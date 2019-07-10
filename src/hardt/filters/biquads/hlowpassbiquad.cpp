#ifndef __HLOWPASSBIQUAD_CPP
#define __HLOWPASSBIQUAD_CPP

#include "hlowpassbiquad.h"

template <class T>
HIirFilter<T>* HLowpassBiQuad<T>::Create(HWriter<T>* writer, size_t blocksize)
{
    // Todo: Calculate correct values
    a0 = 0;
    a1 = 0;
    a2 = 0;
    b0 = 0;
    b1 = 0;
    b2 = 0;

    return new HIirFilter<T>(writer, Normalize(), 5, blocksize);
}

#endif
