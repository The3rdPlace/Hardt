#ifndef __HVFO_CPP
#define __HVFO_CPP

#include "hvfo.h"

template <class T>
HVfo<T>::HVfo(std::string id, H_SAMPLE_RATE rate, int frequency, T amplitude, float phase):
    HGenerator<T>::HGenerator(id, rate, frequency, amplitude, phase),
    _rate(rate),
    _amplitude(amplitude)
{
    SetFrequency(frequency, phase);
}

template <class T>
void HVfo<T>::SetFrequency(int frequency, float phase)
{
    HGenerator<T>::Calculate(frequency, _amplitude, phase);
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HVfo
template
HVfo<int8_t>::HVfo(std::string id, H_SAMPLE_RATE rate, int frequency, int8_t amplitude, float phase);

template
HVfo<uint8_t>::HVfo(std::string id, H_SAMPLE_RATE rate, int frequency, uint8_t amplitude, float phase);

template
HVfo<int16_t>::HVfo(std::string id, H_SAMPLE_RATE rate, int frequency, int16_t amplitude, float phase);

template
HVfo<int32_t>::HVfo(std::string id, H_SAMPLE_RATE rate, int frequency, int32_t amplitude, float phase);

template
HVfo<float>::HVfo(std::string id, H_SAMPLE_RATE rate, int frequency, float amplitude, float phase);

template
HVfo<double>::HVfo(std::string id, H_SAMPLE_RATE rate, int frequency, double amplitude, float phase);

// SetFrequency
template
void HVfo<int8_t>::SetFrequency(int frequency, float phase);

template
void HVfo<uint8_t>::SetFrequency(int frequency, float phase);

template
void HVfo<int16_t>::SetFrequency(int frequency, float phase);

template
void HVfo<int32_t>::SetFrequency(int frequency, float phase);

template
void HVfo<float>::SetFrequency(int frequency, float phase);

template
void HVfo<double>::SetFrequency(int frequency, float phase);

//! @endcond
#endif