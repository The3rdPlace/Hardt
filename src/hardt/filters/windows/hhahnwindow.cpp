#ifndef __HHAHNWINDOW_CPP
#define __HHAHNWINDOW_CPP

#include "hhahnwindow.h"

#include <math.h>

// See https://en.wikipedia.org/wiki/Window_function for in-depth information

template <class T>
float HHahnWindow<T>::ValueAt(int N, int n)
{
    // Hahn window constant alpha
    const float a = 0.5;

    // Return the hahn window value at point 'n'
    return pow( sin( (M_PI * n) / N ), 2 );
}

/********************************************************************
Explicit instantiation
********************************************************************/

template
float HHahnWindow<int8_t>::ValueAt(int N, int n);

template
float HHahnWindow<uint8_t>::ValueAt(int N, int n);

template
float HHahnWindow<int16_t>::ValueAt(int N, int n);

template
float HHahnWindow<int32_t>::ValueAt(int N, int n);

#endif