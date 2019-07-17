#ifndef __HGOERTZELFILTER_CPP
#define __HGOERTZELFILTER_CPP

#include "hgoertzelfilter.h"

template <class T>
HGoertzelFilter<T>::HGoertzelFilter(int size, int average, int bin, HWriter<int>* writer, HWindow<T>* window):
    HOutput<T, int>(writer, size),
    _size(size),
    _average(average),
    _bin(bin),
    _count(0),
    _magnitude(0),
    _window(window)
{
    HLog("HGoerzelFilter(%d, %d, %d, ...)", size, average, bin);

    // Allocate a buffer for intermediate results
    _buffer = new T[size];

    // Set window size
    _window->SetSize(size);
}

template <class T>
int HGoertzelFilter<T>::Output(T* src, size_t size)
{
    // Run the input buffer through a window function, if any is given
    _window->Apply(src, _buffer, size);

    // Todo: Calculate output value
    _magnitude += 1;

    // Time to report the total summed result ?
    if( ++_count >= _average )
    {
        HOutput<T, int>::Ready(&_magnitude, 1);

        // Reset result and counter
        _count = 0;
        _magnitude = 0;
    }

    // We took the entire window
    return size;
}

/********************************************************************
Explicit instantiation
********************************************************************/

template
HGoertzelFilter<int8_t>::HGoertzelFilter(int size, int average, int bin, HWriter<int>* writer, HWindow<int8_t>* window);

template
HGoertzelFilter<uint8_t>::HGoertzelFilter(int size, int average, int bin, HWriter<int>* writer, HWindow<uint8_t>* window);

template
HGoertzelFilter<int16_t>::HGoertzelFilter(int size, int average, int bin, HWriter<int>* writer, HWindow<int16_t>* window);

template
HGoertzelFilter<int32_t>::HGoertzelFilter(int size, int average, int bin, HWriter<int>* writer, HWindow<int32_t>* window);

template
int HGoertzelFilter<int8_t>::Output(int8_t* src, size_t size);

template
int HGoertzelFilter<uint8_t>::Output(uint8_t* src, size_t size);

template
int HGoertzelFilter<int16_t>::Output(int16_t* src, size_t size);

template
int HGoertzelFilter<int32_t>::Output(int32_t* src, size_t size);
#endif
