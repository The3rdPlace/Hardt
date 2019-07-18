#ifndef __HGOERTZELFILTER_CPP
#define __HGOERTZELFILTER_CPP

#include "hgoertzelfilter.h"

template <class T>
HGoertzelFilter<T>::HGoertzelFilter(int size, int average, float bin, HWriter<HGoertzelFilterResult>* writer, HWindow<T>* window):
    HOutput<T, HGoertzelFilterResult>(writer, size),
    _size(size),
    _average(average),
    _bin(bin),
    _count(0),
    _window(window)
{
    HLog("HGoerzelFilter(%f, %d, %f, ...)", size, average, bin);
    Init();
}

template <class T>
HGoertzelFilter<T>::HGoertzelFilter(int size, int average, H_SAMPLE_RATE rate, int frequency, HWriter<HGoertzelFilterResult>* writer, HWindow<T>* window):
    HOutput<T, HGoertzelFilterResult>(writer, size),
    _size(size),
    _average(average),
    _bin( (float) frequency / ((float) rate / (float) size) ),
    _count(0),
    _window(window)
{
    HLog("HGoerzelFilter(%f, %d, %d, %d, ...)", size, average, rate, frequency);
    HLog("Bin number set to %f for frequency %d", _bin, frequency);
    Init();
}

template <class T>
void HGoertzelFilter<T>::Init()
{
    // Allocate a buffer for intermediate results
    _buffer = new T[_size];

    // Initialize results
    _result.Magnitude = 0;
    _result.Phase = 0;

    // Set window size
    _window->SetSize(_size);

    // Calculate constant values
    omega = (2.0f * M_PI * _bin) / _size;
    sine = sin(omega);
    cosine = cos(omega);
    coeff = 2.0f * cosine;

    // For phase calculations
    rad2degr = (float) 180 / M_PI;
}

template <class T>
int HGoertzelFilter<T>::Output(T* src, size_t size)
{
    // Run the input buffer through a window function, if any is given
    _window->Apply(src, _buffer, size);

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
    _result.Magnitude += sqrtf(real*real + imag*imag);

    // Time to report the total summed result ?
    if( ++_count >= _average )
    {
        // Average the magnitude
        _result.Magnitude = _result.Magnitude / _average;

        // Phase can not be averaged, calculate the last phase value
        _result.Phase = ceil(atan2(imag, real) * rad2degr);

        // Send the result
        HOutput<T, HGoertzelFilterResult>::Ready(&_result, 1);

        // Reset result and counter
        _result.Magnitude = 0;
        _result.Phase = 0;
        _count = 0;
    }

    // We took the entire window
    return size;
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HGoertzelFilter
template
HGoertzelFilter<int8_t>::HGoertzelFilter(int size, int average, float bin, HWriter<HGoertzelFilterResult>* writer, HWindow<int8_t>* window);

template
HGoertzelFilter<uint8_t>::HGoertzelFilter(int size, int average, float bin, HWriter<HGoertzelFilterResult>* writer, HWindow<uint8_t>* window);

template
HGoertzelFilter<int16_t>::HGoertzelFilter(int size, int average, float bin, HWriter<HGoertzelFilterResult>* writer, HWindow<int16_t>* window);

template
HGoertzelFilter<int32_t>::HGoertzelFilter(int size, int average, float bin, HWriter<HGoertzelFilterResult>* writer, HWindow<int32_t>* window);

template
HGoertzelFilter<int8_t>::HGoertzelFilter(int size, int average, H_SAMPLE_RATE rate, int frequency, HWriter<HGoertzelFilterResult>* writer, HWindow<int8_t>* window);

template
HGoertzelFilter<uint8_t>::HGoertzelFilter(int size, int average, H_SAMPLE_RATE rate, int frequency, HWriter<HGoertzelFilterResult>* writer, HWindow<uint8_t>* window);

template
HGoertzelFilter<int16_t>::HGoertzelFilter(int size, int average, H_SAMPLE_RATE rate, int frequency, HWriter<HGoertzelFilterResult>* writer, HWindow<int16_t>* window);

template
HGoertzelFilter<int32_t>::HGoertzelFilter(int size, int average, H_SAMPLE_RATE rate, int frequency, HWriter<HGoertzelFilterResult>* writer, HWindow<int32_t>* window);

// Output
template
int HGoertzelFilter<int8_t>::Output(int8_t* src, size_t size);

template
int HGoertzelFilter<uint8_t>::Output(uint8_t* src, size_t size);

template
int HGoertzelFilter<int16_t>::Output(int16_t* src, size_t size);

template
int HGoertzelFilter<int32_t>::Output(int32_t* src, size_t size);

// Init
template
void HGoertzelFilter<int8_t>::Init();

template
void HGoertzelFilter<uint8_t>::Init();

template
void HGoertzelFilter<int16_t>::Init();

template
void HGoertzelFilter<int32_t>::Init();

#endif
