#ifndef __HVFO_CPP
#define __HVFO_CPP

#include "hvfo.h"

template <class T>
HVfo<T>::HVfo(H_SAMPLE_RATE rate, int frequency, T amplitude, float phase):
    HGenerator<T>::HGenerator(rate, frequency, amplitude, phase),
    _rate(rate),
    _amplitude(amplitude)
{
    SetFrequency(frequency, phase);
}

template <class T>
void HVfo<T>::SetFrequency(int frequency, float phase)
{
    HGenerator<T>::Calculate(_rate, frequency, _amplitude, phase);
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HVfo
template
HVfo<int8_t>::HVfo(H_SAMPLE_RATE rate, int frequency, int8_t amplitude, float phase);

template
HVfo<uint8_t>::HVfo(H_SAMPLE_RATE rate, int frequency, uint8_t amplitude, float phase);

template
HVfo<int16_t>::HVfo(H_SAMPLE_RATE rate, int frequency, int16_t amplitude, float phase);

template
HVfo<int32_t>::HVfo(H_SAMPLE_RATE rate, int frequency, int32_t amplitude, float phase);

// Read
/*template
int HVfo<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HVfo<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HVfo<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HVfo<int32_t>::Read(int32_t* dest, size_t blocksize);*/

// SetFrequency
template
void HVfo<int8_t>::SetFrequency(int frequency, float phase);

template
void HVfo<uint8_t>::SetFrequency(int frequency, float phase);

template
void HVfo<int16_t>::SetFrequency(int frequency, float phase);

template
void HVfo<int32_t>::SetFrequency(int frequency, float phase);

#endif