#ifndef __HBASEBAND_CPP
#define __HBASEBAND_CPP

#include "hbaseband.h"

template <class T>
HBaseband<T>::HBaseband(HWriter<T>* writer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(writer, blocksize, probe),
    _rate(rate),
    _center(center),
    _width(width)
{
    HLog("HBaseband(HWriter*, %d, %d, %d, %d)", rate, center, width, blocksize);
}

template <class T>
HBaseband<T>::HBaseband(HWriterConsumer<T>* consumer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(consumer, blocksize, probe),
    _rate(rate),
    _center(center),
    _width(width)
{
    HLog("HBaseband(HWriterConsumer*, %d, %%d, %d, %d)", rate, center, width, blocksize);
}

template <class T>
HBaseband<T>::HBaseband(HReader<T>* reader, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<T>* probe):
    HFilter<T>(reader, blocksize, probe),
    _rate(rate),
    _center(center),
    _width(width)
{
    HLog("HBaseband(HReader*, %d, %d, %d, %d)", rate, center, width, blocksize);
}

template <class T>
HBaseband<T>::~HBaseband()
{
    HLog("~HBaseband()");
}

template <class T>
void HBaseband<T>::Filter(T* src, T* dest, size_t blocksize)
{
    // Todo: Optimize !


    std::complex<double> translated[blocksize];
    std::complex<double> spectrum[blocksize];
    HFft<T> fft(blocksize);

    // Translate
    double x = _center - (_width / 2);
    double y = _rate;
    for( int i = 0; i < blocksize; i++ ) {
        translated[i] = std::complex<double>((double) src[i] * cos( ((double) 2 * M_PI * (double) x * (double) i) / (double) y), -1.0 * (double) src[i] * sin( ((double) 2 * M_PI * (double) x * (double) i) / (double) y ));
    }

    // Filter
    int factor = (_rate / 2) / _width;
    fft.FFT(translated, spectrum);
    for( int i = blocksize / (factor * 2); i < blocksize; i++) {
        spectrum[i] = 0;
    }
    fft.IFFT(spectrum, dest);
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HBaseband
template
HBaseband<int8_t>::HBaseband(HWriter<int8_t>* writer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<int8_t>* probe);

template
HBaseband<uint8_t>::HBaseband(HWriter<uint8_t>* writer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<uint8_t>* probe);

template
HBaseband<int16_t>::HBaseband(HWriter<int16_t>* writer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<int16_t>* probe);

template
HBaseband<int32_t>::HBaseband(HWriter<int32_t>* writer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<int32_t>* probe);

template
HBaseband<int8_t>::HBaseband(HWriterConsumer<int8_t>* consumer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<int8_t>* probe);

template
HBaseband<uint8_t>::HBaseband(HWriterConsumer<uint8_t>* consumer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<uint8_t>* probe);

template
HBaseband<int16_t>::HBaseband(HWriterConsumer<int16_t>* consumer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<int16_t>* probe);

template
HBaseband<int32_t>::HBaseband(HWriterConsumer<int32_t>* consumer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<int32_t>* probe);

template
HBaseband<int8_t>::HBaseband(HReader<int8_t>* reader, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<int8_t>* probe);

template
HBaseband<uint8_t>::HBaseband(HReader<uint8_t>* reader, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<uint8_t>* probe);

template
HBaseband<int16_t>::HBaseband(HReader<int16_t>* reader, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<int16_t>* probe);

template
HBaseband<int32_t>::HBaseband(HReader<int32_t>* reader, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<int32_t>* probe);

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

//! @endcond
#endif