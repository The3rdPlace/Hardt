#ifndef __HFIRDECIMATOR_CPP
#define __HFIRDECIMATOR_CPP

#include "hfirdecimator.h"

template <class T>
HFirDecimator<T>::HFirDecimator(HWriter<T>* writer, int factor, float* coefficients, int points, size_t blocksize):
    HDecimator<T>(writer, factor, blocksize),
    _factor(factor),
    _points(points),
    _size(blocksize) {

    HLog("HFirDecimator(HWriter*, float*, points=%d, blocksize=%d)", points, blocksize);
    Init(coefficients);
}

template <class T>
HFirDecimator<T>::HFirDecimator(HWriterConsumer<T>* consumer, int factor, float* coefficients, int points, size_t blocksize):
    HDecimator<T>(consumer, factor, blocksize),
    _factor(factor),
    _points(points),
    _size(blocksize) {

    HLog("HFirDecimator(HWriterConsumer*, float*, points=%d, blocksize=%d)", points, blocksize);
    Init(coefficients);
    consumer->SetWriter(this);
}

template <class T>
HFirDecimator<T>::HFirDecimator(HReader<T>* reader, int factor, float* coefficients, int points, size_t blocksize):
    HDecimator<T>(reader, factor, blocksize),
    _factor(factor),
    _points(points),
    _size(blocksize) {

    HLog("HFirDecimator(HReader*, float*, points=%d, blocksize=%d)", points, blocksize);
    Init(coefficients);
}

template <class T>
HFirDecimator<T>::~HFirDecimator() {
    delete _buffer;
    delete _fir;
}

template <class T>
void HFirDecimator<T>::Init(float* coefficients) {
    _buffer = new T[_size];
    _fir = new HFir<T>(coefficients, _points, 1, _factor);
}

template <class T>
int HFirDecimator<T>::Write(T* src, size_t blocksize)
{
    std::cout << "FIRDEC WRITE " << blocksize << std::endl;
    T results[blocksize];
    memset((void*) results, 0, sizeof(T) * blocksize);
    _fir->Filter(src, results, blocksize);
    for( int i = 0; i < blocksize; i++ ) {
        std::cout << "FIRDEC [" << i << "] = " << std::to_string(results[i]) << std::endl;
    }
    return HDecimator<T>::Write(results, blocksize);
}

template <class T>
int HFirDecimator<T>::Read(T* dest, size_t blocksize)
{

    std::cout << "FIRDEC READ " << blocksize << std::endl;
    T results[blocksize];
    if( HDecimator<T>::Read(results, blocksize) == 0 ) {
        std::cout << "FIRDEC zero read" << std::endl;
        return 0;
    }

    _fir->Filter(results, dest, blocksize);

    std::cout << "read " << blocksize << std::endl;


    return blocksize;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HDecimator()
template
HFirDecimator<int8_t>::HFirDecimator(HWriter<int8_t>* writer, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<uint8_t>::HFirDecimator(HWriter<uint8_t>* writer, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<int16_t>::HFirDecimator(HWriter<int16_t>* writer, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<int32_t>::HFirDecimator(HWriter<int32_t>* writer, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<int8_t>::HFirDecimator(HWriterConsumer<int8_t>* consumer, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<uint8_t>::HFirDecimator(HWriterConsumer<uint8_t>* consumer, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<int16_t>::HFirDecimator(HWriterConsumer<int16_t>* consumer, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<int32_t>::HFirDecimator(HWriterConsumer<int32_t>* consumer, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<int8_t>::HFirDecimator(HReader<int8_t>* reader, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<uint8_t>::HFirDecimator(HReader<uint8_t>* reader, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<int16_t>::HFirDecimator(HReader<int16_t>* reader, int factor, float* coefficients, int points, size_t blocksize);

template
HFirDecimator<int32_t>::HFirDecimator(HReader<int32_t>* reader, int factor, float* coefficients, int points, size_t blocksize);

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
void HFirDecimator<int8_t>::Init(float* coefficients);

template
void HFirDecimator<uint8_t>::Init(float* coefficients);

template
void HFirDecimator<int16_t>::Init(float* coefficients);

template
void HFirDecimator<int32_t>::Init(float* coefficients);

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
