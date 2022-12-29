#ifndef __HBASEBAND_CPP
#define __HBASEBAND_CPP

#include "hbaseband.h"

template <class T>
HBaseband<T>::HBaseband(std::string id, HWriter<T>* writer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(id, writer, blocksize, probe),
    _blocksize(blocksize),
    _rate(rate),
    _center(center),
    _width(width)
{
    HLog("HBaseband(HWriter*, %d, %d, %d, %d)", rate, center, width, blocksize);

    Init();
}

template <class T>
HBaseband<T>::HBaseband(std::string id, HWriterConsumer<T>* consumer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(id, consumer, blocksize, probe),
    _blocksize(blocksize),
    _rate(rate),
    _center(center),
    _width(width)
{
    HLog("HBaseband(HWriterConsumer*, %d, %%d, %d, %d)", rate, center, width, blocksize);

    Init();
    consumer->SetWriter(this->Writer());
}

template <class T>
HBaseband<T>::HBaseband(std::string id, HReader<T>* reader, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(id, reader, blocksize, probe),
    _blocksize(blocksize),
    _rate(rate),
    _center(center),
    _width(width)
{
    HLog("HBaseband(HReader*, %d, %d, %d, %d)", rate, center, width, blocksize);

    Init();
}

template <class T>
HBaseband<T>::~HBaseband()
{
    HLog("~HBaseband()");

    delete _translated;
    delete _spectrum;
    delete _fft;

    delete _cos;
    delete _sin;
}

template <class T>
void HBaseband<T>::Filter(T* src, T* dest, size_t blocksize)
{
    // Sanity check
    if( blocksize != _blocksize ) {
        throw new HFilterIOException("A HBaseband filter can not be called with other blocksizes than the one it was initialized with");
    }

    // Translate
    for( int i = 0; i < blocksize; i++ ) {
        _translated[i] = std::complex<T>(src[i] * _cos[i], src[i] * _sin[i]);
    }
    _fft->FFT(_translated, _spectrum);

    // Filter
    int factor = (_rate / 2) / _width;
    _fft->FFT(_translated, _spectrum);
    for( int i = blocksize / (factor * 2); i < blocksize; i++) {
        _spectrum[i] = 0;
    }
    _fft->IFFT(_spectrum, dest);
}

template <class T>
void HBaseband<T>::Init() {

    _translated = new std::complex<T>[_blocksize];
    _spectrum = new std::complex<double>[_blocksize];
    _fft = new HFft<T>(_blocksize);

    _cos = new double[_blocksize];
    _sin = new double[_blocksize];

    PreCalculate();
}

template <class T>
void HBaseband<T>::PreCalculate() {

    // Translation moves the center frequency towards
    // zero, so to move a segment of width W with center C
    // down towards zero so that Wmin = 0, we need to use a
    // center of C - W/2 = Wmin - 0 = Wmin
    double center = _center - (_width / 2);

    for( int i = 0; i < _blocksize; i++ ) {
        double arg = (2.0 * M_PI * (double) center * i) / (double) _rate;
        _cos[i] =        cos( arg );
        _sin[i] = -1.0 * sin( arg );
    }
}

template <class T>
void HBaseband<T>::SetSegment(int center, int width) {
    _center = center;
    _width = width;
    PreCalculate();
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HBaseband
template
HBaseband<int8_t>::HBaseband(std::string id, HWriter<int8_t>* writer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<int8_t>* probe);

template
HBaseband<uint8_t>::HBaseband(std::string id, HWriter<uint8_t>* writer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<uint8_t>* probe);

template
HBaseband<int16_t>::HBaseband(std::string id, HWriter<int16_t>* writer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<int16_t>* probe);

template
HBaseband<int32_t>::HBaseband(std::string id, HWriter<int32_t>* writer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<int32_t>* probe);

template
HBaseband<int8_t>::HBaseband(std::string id, HWriterConsumer<int8_t>* consumer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<int8_t>* probe);

template
HBaseband<uint8_t>::HBaseband(std::string id, HWriterConsumer<uint8_t>* consumer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<uint8_t>* probe);

template
HBaseband<int16_t>::HBaseband(std::string id, HWriterConsumer<int16_t>* consumer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<int16_t>* probe);

template
HBaseband<int32_t>::HBaseband(std::string id, HWriterConsumer<int32_t>* consumer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<int32_t>* probe);

template
HBaseband<int8_t>::HBaseband(std::string id, HReader<int8_t>* reader, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<int8_t>* probe);

template
HBaseband<uint8_t>::HBaseband(std::string id, HReader<uint8_t>* reader, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<uint8_t>* probe);

template
HBaseband<int16_t>::HBaseband(std::string id, HReader<int16_t>* reader, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<int16_t>* probe);

template
HBaseband<int32_t>::HBaseband(std::string id, HReader<int32_t>* reader, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<int32_t>* probe);

// ~HBaseband()
template
HBaseband<int8_t>::~HBaseband();

template
HBaseband<uint8_t>::~HBaseband();

template
HBaseband<int16_t>::~HBaseband();

template
HBaseband<int32_t>::~HBaseband();

// Filter
template
void HBaseband<int8_t>::Filter(int8_t* src, int8_t* dest, size_t blocksize);

template
void HBaseband<uint8_t>::Filter(uint8_t* src, uint8_t* dest, size_t blocksize);

template
void HBaseband<int16_t>::Filter(int16_t* src, int16_t* dest, size_t blocksize);

template
void HBaseband<int32_t>::Filter(int32_t* src, int32_t* dest, size_t blocksize);

// Init
template
void HBaseband<int8_t>::Init();

template
void HBaseband<uint8_t>::Init();

template
void HBaseband<int16_t>::Init();

template
void HBaseband<int32_t>::Init();

// PreCalculate
template
void HBaseband<int8_t>::PreCalculate();

template
void HBaseband<uint8_t>::PreCalculate();

template
void HBaseband<int16_t>::PreCalculate();

template
void HBaseband<int32_t>::PreCalculate();

// SetSegment
template
void HBaseband<int8_t>::SetSegment(int center, int width);

template
void HBaseband<uint8_t>::SetSegment(int center, int width);

template
void HBaseband<int16_t>::SetSegment(int center, int width);

template
void HBaseband<int32_t>::SetSegment(int center, int width);

//! @endcond
#endif