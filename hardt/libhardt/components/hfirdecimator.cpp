#ifndef __HFIRDECIMATOR_CPP
#define __HFIRDECIMATOR_CPP

#include "hfirdecimator.h"

template <class T>
HFirDecimator<T>::HFirDecimator(HWriter<T>* writer, int factor, size_t blocksize):
    HDecimator<T>(writer, 1, blocksize),
    _factor(factor) {

    HLog("HFirDecimator(HWriter*, blocksize=%d)", blocksize);
    Init();
}

template <class T>
HFirDecimator<T>::HFirDecimator(HWriterConsumer<T>* consumer, int factor, size_t blocksize):
    HDecimator<T>(consumer, 1, blocksize),
    _factor(factor) {

    HLog("HFirDecimator(HWriterConsumer*, blocksize=%d)", blocksize);
    Init();
    consumer->SetWriter(this);
}

template <class T>
HFirDecimator<T>::HFirDecimator(HReader<T>* reader, int factor, size_t blocksize):
    HDecimator<T>(reader, 1, blocksize),
    _factor(factor){

    HLog("HFirDecimator(HReader*, blocksize=%d)", blocksize);
    Init();
}

template <class T>
HFirDecimator<T>::~HFirDecimator() {
}

template <class T>
void HFirDecimator<T>::Init() {
}

template <class T>
int HFirDecimator<T>::Write(T* src, size_t blocksize)
{
    T results[blocksize / _factor];
    for( int i = 0; i < blocksize; i += _factor ) {
        results[i] = 0;
    }
    return HDecimator<T>::Write(results, blocksize / _factor);
}

template <class T>
int HFirDecimator<T>::Read(T* dest, size_t blocksize)
{
    return HDecimator<T>::Read(dest, blocksize);
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HDecimator()
template
HFirDecimator<int8_t>::HFirDecimator(HWriter<int8_t>* writer, int factor, size_t blocksize);

template
HFirDecimator<uint8_t>::HFirDecimator(HWriter<uint8_t>* writer, int factor, size_t blocksize);

template
HFirDecimator<int16_t>::HFirDecimator(HWriter<int16_t>* writer, int factor, size_t blocksize);

template
HFirDecimator<int32_t>::HFirDecimator(HWriter<int32_t>* writer, int factor, size_t blocksize);

template
HFirDecimator<int8_t>::HFirDecimator(HWriterConsumer<int8_t>* consumer, int factor, size_t blocksize);

template
HFirDecimator<uint8_t>::HFirDecimator(HWriterConsumer<uint8_t>* consumer, int factor, size_t blocksize);

template
HFirDecimator<int16_t>::HFirDecimator(HWriterConsumer<int16_t>* consumer, int factor, size_t blocksize);

template
HFirDecimator<int32_t>::HFirDecimator(HWriterConsumer<int32_t>* consumer, int factor, size_t blocksize);

template
HFirDecimator<int8_t>::HFirDecimator(HReader<int8_t>* reader, int factor, size_t blocksize);

template
HFirDecimator<uint8_t>::HFirDecimator(HReader<uint8_t>* reader, int factor, size_t blocksize);

template
HFirDecimator<int16_t>::HFirDecimator(HReader<int16_t>* reader, int factor, size_t blocksize);

template
HFirDecimator<int32_t>::HFirDecimator(HReader<int32_t>* reader, int factor, size_t blocksize);

// ~HDecimator()
template
HFirDecimator<int8_t>::~HFirDecimator();

template
HFirDecimator<uint8_t>::~HFirDecimator();

template
HFirDecimator<int16_t>::~HFirDecimator();

template
HFirDecimator<int32_t>::~HFirDecimator();

// Init()
template
void HFirDecimator<int8_t>::Init();

template
void HFirDecimator<uint8_t>::Init();

template
void HFirDecimator<int16_t>::Init();

template
void HFirDecimator<int32_t>::Init();

// Write()
template
int HFirDecimator<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HFirDecimator<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HFirDecimator<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HFirDecimator<int32_t>::Write(int32_t* src, size_t blocksize);

// Read()
template
int HFirDecimator<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HFirDecimator<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HFirDecimator<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HFirDecimator<int32_t>::Read(int32_t* dest, size_t blocksize);

//! @endcond
#endif
