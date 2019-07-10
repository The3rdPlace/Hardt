#ifndef __HLOWPASSBIQUAD_CPP
#define __HLOWPASSBIQUAD_CPP

#include "hlowpassbiquad.h"

template <class T>
float* HLowpassBiQuad<T>::Calculate()
{
    a0 = 1 + alpha;
    a1 = -2 * omegaC;
    a2 = 1 - alpha;
    b0 = (1 - omegaC) / 2;
    b1 = 1 - omegaC;
    b2 = (1 - omegaC) / 2;

    return Normalize();
}

template <class T>
HIirFilter<T>* HLowpassBiQuad<T>::Create(HWriter<T>* writer, float fCutOff, float rate, float quality, float gain, size_t blocksize)
{
    Initialize(fCutOff, rate, quality, gain);

    return new HIirFilter<T>(writer, Calculate(), 5, blocksize);
}

template <class T>
HIirFilter<T>* HLowpassBiQuad<T>::Create(HReader<T>* reader, float fCutOff, float rate, float quality, float gain,  size_t blocksize)
{
    Initialize(fCutOff, rate, quality, gain);

    return new HIirFilter<T>(reader, Calculate(), 5, blocksize);
}

#endif
