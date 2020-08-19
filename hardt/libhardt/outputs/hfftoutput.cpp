#ifndef __HFFTOUTPUT_CPP
#define __HFFTOUTPUT_CPP

#include "hfftoutput.h"

#include <cstring>
#include <complex>
#include <valarray>

template <class T>
HFftOutput<T>::HFftOutput(int size, int average, HWriter<HFftResults>* writer, HWindow<T>* window, int zoomPoints, H_SAMPLE_RATE zoomRate, int zoomFactor, int zoomCenter):
    HOutput<T, HFftResults>(writer, size),
    _size(size),
    _average(average),
    _count(0),
    _window(window),
    _zoomPoints(zoomPoints),
    _zoomRate(zoomRate),
    _zoomFactor(zoomFactor),
    _zoomCenter(zoomCenter),
    _zoomEnabled(false)
{
    HLog("HFftOutput(%d, %d, ..., %d, %d, %d, %d)", size, average, zoomPoints, zoomRate, zoomFactor, zoomCenter);
    Init();
}

template <class T>
HFftOutput<T>::HFftOutput(int size, int average, HWriterConsumer<T>* consumer, HWindow<T>* window, int zoomPoints, H_SAMPLE_RATE zoomRate, int zoomFactor, int zoomCenter):
    HOutput<T, HFftResults>(size, consumer),
    _size(size),
    _average(average),
    _count(0),
    _window(window),
    _zoomPoints(zoomPoints),
    _zoomRate(zoomRate),
    _zoomFactor(zoomFactor),
    _zoomCenter(zoomCenter),
    _zoomEnabled(false)
{
    HLog("HFftOutput(%d, %d, ..., %d, %d, %d)", size, average, zoomPoints, zoomRate, zoomFactor, zoomCenter);
    Init();
}

template <class T>
void HFftOutput<T>::Init()
{
    // Allocate a buffer for the spectrum and phase values
    _spectrum = new double[_size];
    memset((void*) _spectrum, 0, _size * sizeof(double));
    _fftResult = new std::complex<double>[_size];
    _result = new std::complex<double>[_size / 2];
    memset((void*) _result, 0, (_size / 2) * sizeof(std::complex<double>));

    // Allocate a buffer for intermediate results
    _buffer = new T[_size];

    // Set window size
    _window->SetSize(_size);

    // Create the fft
    _fft = new HFft<T>(_size);//, _window);

    // Setup zooming - even if the factor is 1 at the moment
    if( _zoomPoints != 0 && _zoomRate != 0 ) {
        _zoomEnabled = true;

        // Check that the requested window is sane
        if( Left() < 0 || Right() > _zoomRate / 2) {
            throw new HInitializationException("The left or right bound of the zoomed frequency spectrum falls outside the range supported by the sampling rate");
        }

        // If center = 0, we should use the middle of the input spectrum
        if( _zoomCenter == 0 ) {
            _zoomCenter = _zoomRate / 4;
        }

        // Setup zoom chain
        _zoomInputWriter = new HInputWriter<T>();
        _zoomBaseband = new HBaseband<T>(_zoomInputWriter->Consumer(), _zoomRate, _zoomCenter, (_zoomRate / 2) / _zoomFactor, _size);
        _zoomDecimator = new HDecimator<T>(_zoomBaseband->Consumer(), _zoomFactor, _size);
        _zoomOutput = new T[_size];
        _zoomMemoryWriter = new HMemoryWriter<T>(_zoomDecimator->Consumer(), _zoomOutput, _size);
    }
}

template <class T>
void HFftOutput<T>::SetZoom(int zoomFactor, int zoomCenter) {

    if( _zoomEnabled ) {

        // Store new settings
        _zoomFactor = zoomFactor;
        if( zoomCenter == 0 ) {
            _zoomCenter = _zoomRate / 4;
        } else {
            _zoomCenter = zoomCenter;
        }

        // Set new zoom settings
        _zoomBaseband->SetSegment(_zoomCenter, (_zoomRate / 2) / _zoomFactor);
        _zoomDecimator->SetFactor(_zoomFactor);
    } else {
        throw new HInitializationException("You can not set zooming on a HFftOutput not initialized with a zoomrate and zoompoints!");
    }
}

template <class T>
T* HFftOutput<T>::Zoom(T* src, size_t size) {

    // No zooming ?
    if( _zoomFactor == 1 || !_zoomEnabled ) {
        return src;
    }

    _zoomInputWriter->Write(src, size);
    if( _zoomMemoryWriter->GetPosition() >= size ) {
        _zoomMemoryWriter->Reset();
        return _zoomOutput;
    }
    return nullptr;

}

template <class T>
int HFftOutput<T>::Output(T* src, size_t size)
{
    T* input = Zoom(src, size);

    // Check if zooming returned any data - which it will not for a number of
    // calls when zooming (depending on the decimation factcor)
    if( input == nullptr ) {
        return size;
    }

    // Get the spectrum of the (possibly zoomed) input data
    // and add to the averaging buffer
    _fft->FFT(input, _fftResult);
    for( int i = 0; i < size / 2; i++ ) {
        _result[i] += _fftResult[i];
    }

    // Did we reach averaging target ?
    if( ++_count >= _average )
    {
        // Calculate spectrum
        for( int i = 0; i < size / 2; i++ )
        {
            // Get absolute value at point n
            double value = std::abs(_result[i]);

            // Spectrum values
            _spectrum[i] += value / _average;
        }

        // Call the callback function with the calculated spectrum
        // Note that this FFT algorithm reverses the spectrum (even and odd is reversed)
        // so we need to return the first N/2 bins, not the second part of the spectrum
        HFftResults results;
        results.Spectrum = &_spectrum[0];
        results.Result = &_result[0];
        results.Size = size / 2;
        HOutput<T, HFftResults>::Ready(&results, 1);

        // Reset results
        _count = 0;
        memset((void*) _spectrum, 0, size * sizeof(double));
        memset((void*) _result, 0, (size / 2) * sizeof(std::complex<double>));
    }

    // We took the entire window
    return size;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

template
HFftOutput<int8_t>::HFftOutput(int size, int average, HWriter<HFftResults>* writer, HWindow<int8_t>* window, int zoomFactor, int zoomCenter, int zoomPoints, H_SAMPLE_RATE zoomRate);

template
HFftOutput<uint8_t>::HFftOutput(int size, int average, HWriter<HFftResults>* writer, HWindow<uint8_t>* window, int zoomFactor, int zoomCenter, int zoomPoints, H_SAMPLE_RATE zoomRate);

template
HFftOutput<int16_t>::HFftOutput(int size, int average, HWriter<HFftResults>* writer, HWindow<int16_t>* window, int zoomFactor, int zoomCenter, int zoomPoints, H_SAMPLE_RATE zoomRate);

template
HFftOutput<int32_t>::HFftOutput(int size, int average, HWriter<HFftResults>* writer, HWindow<int32_t>* window, int zoomFactor, int zoomCenter, int zoomPoints, H_SAMPLE_RATE zoomRate);

template
HFftOutput<int8_t>::HFftOutput(int size, int average, HWriterConsumer<int8_t>* consumer, HWindow<int8_t>* window, int zoomFactor, int zoomCenter, int zoomPoints, H_SAMPLE_RATE zoomRate);

template
HFftOutput<uint8_t>::HFftOutput(int size, int average, HWriterConsumer<uint8_t>* consumer, HWindow<uint8_t>* window, int zoomFactor, int zoomCenter, int zoomPoints, H_SAMPLE_RATE zoomRate);

template
HFftOutput<int16_t>::HFftOutput(int size, int average, HWriterConsumer<int16_t>* consumer, HWindow<int16_t>* window, int zoomFactor, int zoomCenter, int zoomPoints, H_SAMPLE_RATE zoomRate);

template
HFftOutput<int32_t>::HFftOutput(int size, int average, HWriterConsumer<int32_t>* consumer, HWindow<int32_t>* window, int zoomFactor, int zoomCenter, int zoomPoints, H_SAMPLE_RATE zoomRate);

template
int HFftOutput<int8_t>::Output(int8_t* src, size_t size);

template
int HFftOutput<uint8_t>::Output(uint8_t* src, size_t size);

template
int HFftOutput<int16_t>::Output(int16_t* src, size_t size);

template
int HFftOutput<int32_t>::Output(int32_t* src, size_t size);

//! @endcond
#endif
