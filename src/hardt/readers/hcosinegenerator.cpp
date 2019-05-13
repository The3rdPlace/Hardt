#ifndef __HCOSINEGENERATOR_CPP
#define __HCOSINEGENERATOR_CPP

#include "hcosinegenerator.h"

template <class T>
HCosineGenerator<T>::HCosineGenerator(H_SAMPLE_RATE rate, int frequency, T amplitude):
    HGenerator<T>::HGenerator(rate, frequency, amplitude, M_PI / 2)
{}

template <class T>
int HCosineGenerator<T>::Read(T* dest, size_t blocksize)
{
    HGenerator<T>::GetSamples(dest, blocksize);
    return blocksize;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HSineGenerator
template
HCosineGenerator<int8_t>::HCosineGenerator(H_SAMPLE_RATE rate, int frequency, int8_t amplitude);

template
HCosineGenerator<uint8_t>::HCosineGenerator(H_SAMPLE_RATE rate, int frequency, uint8_t amplitude);

template
HCosineGenerator<int16_t>::HCosineGenerator(H_SAMPLE_RATE rate, int frequency, int16_t amplitude);

template
HCosineGenerator<int32_t>::HCosineGenerator(H_SAMPLE_RATE rate, int frequency, int32_t amplitude);

// Read
template
int HCosineGenerator<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HCosineGenerator<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HCosineGenerator<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HCosineGenerator<int32_t>::Read(int32_t* dest, size_t blocksize);

#endif