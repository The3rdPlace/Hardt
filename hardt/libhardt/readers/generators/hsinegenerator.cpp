#ifndef __HSINEGENERATOR_CPP
#define __HSINEGENERATOR_CPP

#include "hsinegenerator.h"

template <class T>
HSineGenerator<T>::HSineGenerator(std::string id, H_SAMPLE_RATE rate, int frequency, T amplitude):
    HGenerator<T>::HGenerator(id, rate, frequency, amplitude)
{}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HSineGenerator
template
HSineGenerator<int8_t>::HSineGenerator(std::string id, H_SAMPLE_RATE rate, int frequency, int8_t amplitude);

template
HSineGenerator<uint8_t>::HSineGenerator(std::string id, H_SAMPLE_RATE rate, int frequency, uint8_t amplitude);

template
HSineGenerator<int16_t>::HSineGenerator(std::string id, H_SAMPLE_RATE rate, int frequency, int16_t amplitude);

template
HSineGenerator<int32_t>::HSineGenerator(std::string id, H_SAMPLE_RATE rate, int frequency, int32_t amplitude);

//! @endcond
#endif