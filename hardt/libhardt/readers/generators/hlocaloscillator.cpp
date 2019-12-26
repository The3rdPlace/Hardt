#ifndef __HLOCALOSCILLATOR_CPP
#define __HLOCALOSCILLATOR_CPP

#include "hlocaloscillator.h"

template <class T>
HLocalOscillator<T>::HLocalOscillator(H_SAMPLE_RATE rate, int frequency, int amplitude):
    HVfo<T>::HVfo(rate, frequency, amplitude, M_PI / 2)
{}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HLocalOscilator
template
HLocalOscillator<int8_t>::HLocalOscillator(H_SAMPLE_RATE rate, int frequency, int amplitude);

template
HLocalOscillator<uint8_t>::HLocalOscillator(H_SAMPLE_RATE rate, int frequency, int amplitude);

template
HLocalOscillator<int16_t>::HLocalOscillator(H_SAMPLE_RATE rate, int frequency, int amplitude);

template
HLocalOscillator<int32_t>::HLocalOscillator(H_SAMPLE_RATE rate, int frequency, int amplitude);

template
HLocalOscillator<float>::HLocalOscillator(H_SAMPLE_RATE rate, int frequency, int amplitude);

template
HLocalOscillator<double>::HLocalOscillator(H_SAMPLE_RATE rate, int frequency, int amplitude);

//! @endcond
#endif