#ifndef __HKAISERBESSEL_CPP
#define __HKAISERBESSEL_CPP

#include "hkaiserbessel.h"

template <class T>
HKaiserBessel<T>::HKaiserBessel(int fa, int fb, float rate, int points, int att)
{
    // Todo: Calculate filter coefficients. These will cover lowpass, highpass,
    //       and bandpass filters. Bandstop filters requires further manipulation
    //       which must be done by implementing the pure virtual 'void Calculate()' 
    //       function in the relevant filter class.
}

template <class T>
float* HKaiserBessel<T>::Calculate()
{
    // Todo: Return the filter coefficints
    // return Calculate( coefficients, points );
    return nullptr;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HKaiserBessel
template
HKaiserBessel<int8_t>::HKaiserBessel(int fa, int fb, float rate, int points, int att);

template
HKaiserBessel<uint8_t>::HKaiserBessel(int fa, int fb, float rate, int points, int att);

template
HKaiserBessel<int16_t>::HKaiserBessel(int fa, int fb, float rate, int points, int att);

template
HKaiserBessel<int32_t>::HKaiserBessel(int fa, int fb, float rate, int points, int att);

// Calculate
template
float* HKaiserBessel<int8_t>::Calculate();

template
float* HKaiserBessel<uint8_t>::Calculate();

template
float* HKaiserBessel<int16_t>::Calculate();

template
float* HKaiserBessel<int32_t>::Calculate();

//! @endcond
#endif