#ifndef __HLOCALOSCILLATOR_CPP
#define __HLOCALOSCILLATOR_CPP

#include "hlocaloscillator.h"

template <class T>
HLocalOscillator<T>::HLocalOscillator(H_SAMPLE_RATE rate, int frequency):
    HGenerator<T>::HGenerator(rate, frequency, 1, M_PI / 2)
{}

/********************************************************************
Explicit instantiation
********************************************************************/

// HLocalOscilator
template
HLocalOscillator<float>::HLocalOscillator(H_SAMPLE_RATE rate, int frequency);

template
HLocalOscillator<double>::HLocalOscillator(H_SAMPLE_RATE rate, int frequency);

#endif