#ifndef __HSINEGENERATOR_CPP
#define __HSINEGENERATOR_CPP

#include "hsinegenerator.h"

template <class T>
HSineGenerator<T>::HSineGenerator(H_SAMPLE_RATE rate, int frequency, T amplitude):
    HGenerator<T>::HGenerator(rate, frequency, amplitude)
{}

template <class T>
int HSineGenerator<T>::Read(T* dest, size_t blocksize)
{
    HGenerator<T>::GetSamples(dest, blocksize);
    return blocksize;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HSineGenerator
template
HSineGenerator<int8_t>::HSineGenerator(H_SAMPLE_RATE rate, int frequency, int8_t amplitude);

template
HSineGenerator<uint8_t>::HSineGenerator(H_SAMPLE_RATE rate, int frequency, uint8_t amplitude);

template
HSineGenerator<int16_t>::HSineGenerator(H_SAMPLE_RATE rate, int frequency, int16_t amplitude);

template
HSineGenerator<int32_t>::HSineGenerator(H_SAMPLE_RATE rate, int frequency, int32_t amplitude);

// Read
template
int HSineGenerator<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HSineGenerator<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HSineGenerator<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HSineGenerator<int32_t>::Read(int32_t* dest, size_t blocksize);

#endif