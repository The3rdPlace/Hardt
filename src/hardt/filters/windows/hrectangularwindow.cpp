#ifndef __HRECTANGULARWINDOW_CPP
#define __HRECTANGULARWINDOW_CPP

#include "hrectangularwindow.h"

template <class T>
float HRectangularWindow<T>::ValueAt(int N, int n)
{
    return 1;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

template
float HRectangularWindow<int8_t>::ValueAt(int N, int n);

template
float HRectangularWindow<uint8_t>::ValueAt(int N, int n);

template
float HRectangularWindow<int16_t>::ValueAt(int N, int n);

template
float HRectangularWindow<int32_t>::ValueAt(int N, int n);

//! @endcond
#endif