#ifndef __HCOSINEGENERATOR_CPP
#define __HCOSINEGENERATOR_CPP

#include "hcosinegenerator.h"

template <class T>
HCosineGenerator<T>::HCosineGenerator(H_SAMPLE_RATE rate, int frequency, T amplitude):
    HGenerator<T>::HGenerator(rate, frequency, amplitude, M_PI / 2)
{}

/********************************************************************
Explicit instantiation
********************************************************************/

// HCosineGenerator
template
HCosineGenerator<int8_t>::HCosineGenerator(H_SAMPLE_RATE rate, int frequency, int8_t amplitude);

template
HCosineGenerator<uint8_t>::HCosineGenerator(H_SAMPLE_RATE rate, int frequency, uint8_t amplitude);

template
HCosineGenerator<int16_t>::HCosineGenerator(H_SAMPLE_RATE rate, int frequency, int16_t amplitude);

template
HCosineGenerator<int32_t>::HCosineGenerator(H_SAMPLE_RATE rate, int frequency, int32_t amplitude);

#endif