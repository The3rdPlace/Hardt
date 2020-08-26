#ifndef __HWINDOW_CPP
#define __HWINDOW_CPP

#include "hwindow.h"

template <class T>
HWindow<T>::HWindow():
    _N(0),
    _valueAt(NULL)
{}

template <class T>
HWindow<T>::~HWindow()
{
    delete[] _valueAt;
}


template <class T>
void HWindow<T>::SetSize(int N)
{
    // Delete previous set of multiplication factors, if any
    if( _valueAt != NULL )
    {
        delete _valueAt;
    }

    // Store the current N value
    _N = N;

    // Allocate a new buffer for the window multiplication factor for each 'n'
    _valueAt = (float*) new float[N];

    // Get window multiplication factor for each 'n'
    for( int n = 0; n < N; n++ )
    {
        _valueAt[n] = ValueAt(N, n);
    }
}

template <class T>
void HWindow<T>::Apply(T* src, T* dest, size_t blocksize)
{
    if( blocksize != _N )
    {
        HError("Illegal blocksize in Apply() to HWindow. SetSize(%d) but Apply called with %d", _N, blocksize);
        throw new HFilterIOException("It is not allowed to write less or more data than the current size of the window");
    }

    // Calculate sample values after having applied the window at the given 'n'
    for( int n = 0; n < _N; n++ )
    {
        dest[n] = (T) ((float) src[n] * _valueAt[n]);
    }
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HWindow
template
HWindow<int8_t>::HWindow();

template
HWindow<uint8_t>::HWindow();

template
HWindow<int16_t>::HWindow();

template
HWindow<int32_t>::HWindow();

template
HWindow<float>::HWindow();

template
HWindow<double>::HWindow();

// ~HWindow()
template
HWindow<int8_t>::~HWindow();

template
HWindow<uint8_t>::~HWindow();

template
HWindow<int16_t>::~HWindow();

template
HWindow<int32_t>::~HWindow();

template
HWindow<float>::~HWindow();

template
HWindow<double>::~HWindow();

// SetSize()
template
void HWindow<int8_t>::SetSize(int N);

template
void HWindow<uint8_t>::SetSize(int N);

template
void HWindow<int16_t>::SetSize(int N);

template
void HWindow<int32_t>::SetSize(int N);

template
void HWindow<float>::SetSize(int N);

template
void HWindow<double>::SetSize(int N);

// Apply()
template
void HWindow<int8_t>::Apply(int8_t* src, int8_t* dest, size_t blocksize);

template
void HWindow<uint8_t>::Apply(uint8_t* src, uint8_t* dest, size_t blocksize);

template
void HWindow<int16_t>::Apply(int16_t* src, int16_t* dest, size_t blocksize);

template
void HWindow<int32_t>::Apply(int32_t* src, int32_t* dest, size_t blocksize);

template
void HWindow<float>::Apply(float* src, float* dest, size_t blocksize);

template
void HWindow<double>::Apply(double* src, double* dest, size_t blocksize);

//! @endcond
#endif