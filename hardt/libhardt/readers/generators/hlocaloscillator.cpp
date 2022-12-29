#ifndef __HLOCALOSCILLATOR_CPP
#define __HLOCALOSCILLATOR_CPP

#include "hlocaloscillator.h"

template <class T>
HLocalOscillator<T>::HLocalOscillator(std::string id, H_SAMPLE_RATE rate, int frequency, int amplitude, float phase):
    HVfo<T>::HVfo(id, rate, frequency, amplitude, phase) {}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HLocalOscilator
template
HLocalOscillator<int8_t>::HLocalOscillator(std::string id, H_SAMPLE_RATE rate, int frequency, int amplitude, float phase);

template
HLocalOscillator<uint8_t>::HLocalOscillator(std::string id, H_SAMPLE_RATE rate, int frequency, int amplitude, float phase);

template
HLocalOscillator<int16_t>::HLocalOscillator(std::string id, H_SAMPLE_RATE rate, int frequency, int amplitude, float phase);

template
HLocalOscillator<int32_t>::HLocalOscillator(std::string id, H_SAMPLE_RATE rate, int frequency, int amplitude, float phase);

template
HLocalOscillator<float>::HLocalOscillator(std::string id, H_SAMPLE_RATE rate, int frequency, int amplitude, float phase);

template
HLocalOscillator<double>::HLocalOscillator(std::string id, H_SAMPLE_RATE rate, int frequency, int amplitude, float phase);

//! @endcond
#endif