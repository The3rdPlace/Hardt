#ifndef __HGOERTZELFILTER_CPP
#define __HGOERTZELFILTER_CPP

#include "hgoertzelfilter.h"

template <class T>
HGoertzelFilter<T>::HGoertzelFilter(int size, int average, float bin, HWriter<int>* writer, HWindow<T>* window):
    HOutput<T, int>(writer, size),
    _size(size),
    _average(average),
    _bin(bin),
    _count(0),
    _magnitude(0),
    _window(window)
{
    HLog("HGoerzelFilter(%f, %d, %f, ...)", size, average, bin);

    // Allocate a buffer for intermediate results
    _buffer = new T[size];

    // Set window size
    _window->SetSize(size);
}

template <class T>
HGoertzelFilter<T>::HGoertzelFilter(int size, int average, H_SAMPLE_RATE rate, int frequency, HWriter<int>* writer, HWindow<T>* window):
    HOutput<T, int>(writer, size),
    _size(size),
    _average(average),
    _bin( frequency / (rate / size) ),
    _count(0),
    _magnitude(0),
    _window(window)
{
    HLog("HGoerzelFilter(%f, %d, %d, %d, ...)", size, average, rate, frequency);

    // Allocate a buffer for intermediate results
    _buffer = new T[size];

    // Set window size
    _window->SetSize(size);

    // Calculate bin number
    float floatFrequency = frequency;
    float floatRate = rate;
    _bin = floatFrequency / (floatRate / _size);
    HLog("Bin number set to %f for frequency %d", _bin, frequency);
}

template <class T>
int HGoertzelFilter<T>::Output(T* src, size_t size)
{
    // Run the input buffer through a window function, if any is given
    _window->Apply(src, _buffer, size);

    // Todo: Move these to the constructor
    float k = _bin;
    float omega = (2.0f * M_PI * k) / _size;
    float sine = sin(omega);
    float cosine = cos(omega);
    float coeff = 2.0f * cosine;

    // Setup delayed output values
    float q0 = 0;
    float q1 = 0;
    float q2 = 0;

    // Calculate the Goertzel filter output
    for(int i = 0; i < _size; i++)
    {
        q2 = q1;
        q1 = q0;
        q0 = coeff * q1 - q2 + (float) src[i];
    }

    // Get the magnitude
    float real = (q0 - q1 * cosine);
    float imag = (-q1 * sine);
    _magnitude += sqrtf(real*real + imag*imag);

    // Time to report the total summed result ?
    if( ++_count >= _average )
    {
        // Average the magnitude
        _magnitude = _magnitude / _average;

        // Phase can not be averaged, calculate the last phase value
        float phase = atan(imag / real) * (180 / M_PI);

        // Send the result
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
HGoertzelFilter<int8_t>::HGoertzelFilter(int size, int average, float bin, HWriter<int>* writer, HWindow<int8_t>* window);

template
HGoertzelFilter<uint8_t>::HGoertzelFilter(int size, int average, float bin, HWriter<int>* writer, HWindow<uint8_t>* window);

template
HGoertzelFilter<int16_t>::HGoertzelFilter(int size, int average, float bin, HWriter<int>* writer, HWindow<int16_t>* window);

template
HGoertzelFilter<int32_t>::HGoertzelFilter(int size, int average, float bin, HWriter<int>* writer, HWindow<int32_t>* window);

template
HGoertzelFilter<int8_t>::HGoertzelFilter(int size, int average, H_SAMPLE_RATE rate, int frequency, HWriter<int>* writer, HWindow<int8_t>* window);

template
HGoertzelFilter<uint8_t>::HGoertzelFilter(int size, int average, H_SAMPLE_RATE rate, int frequency, HWriter<int>* writer, HWindow<uint8_t>* window);

template
HGoertzelFilter<int16_t>::HGoertzelFilter(int size, int average, H_SAMPLE_RATE rate, int frequency, HWriter<int>* writer, HWindow<int16_t>* window);

template
HGoertzelFilter<int32_t>::HGoertzelFilter(int size, int average, H_SAMPLE_RATE rate, int frequency, HWriter<int>* writer, HWindow<int32_t>* window);

template
int HGoertzelFilter<int8_t>::Output(int8_t* src, size_t size);

template
int HGoertzelFilter<uint8_t>::Output(uint8_t* src, size_t size);

template
int HGoertzelFilter<int16_t>::Output(int16_t* src, size_t size);

template
int HGoertzelFilter<int32_t>::Output(int32_t* src, size_t size);
#endif
