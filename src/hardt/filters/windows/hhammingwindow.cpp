#ifndef __HHAMMINGWINDOW_CPP
#define __HHAMMINGWINDOW_CPP

#include "hhammingwindow.h"

#include <math.h>

template <class T>
float HHammingWindow<T>::ValueAt(int N, int n)
{
    // Hamming window constants, alpha and beta
    const float a = 0.54;
    const float b = 0.23;

    // Return the hamming window value at point 'n'
    return a - (b * cos( (2 * M_PI * n) / N));
}

/********************************************************************
Explicit instantiation
********************************************************************/

template
float HHammingWindow<int8_t>::ValueAt(int N, int n);

template
float HHammingWindow<uint8_t>::ValueAt(int N, int n);

template
float HHammingWindow<int16_t>::ValueAt(int N, int n);

template
float HHammingWindow<int32_t>::ValueAt(int N, int n);

#endif