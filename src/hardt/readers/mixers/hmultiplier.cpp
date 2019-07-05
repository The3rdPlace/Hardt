#ifndef __HMultiplier_CPP
#define __HMultiplier_CPP

#include "hreader.h"
#include "hmultiplier.h"


template <class T>
HMultiplier<T>::HMultiplier(HReader<T>* reader, H_SAMPLE_RATE rate, int frequency, size_t blocksize):
    _reader(reader),
    _blocksize(blocksize)
{
    HLog("HMultiplier(HReader*, %d, %d, %d)", rate, frequency, blocksize);
    _buffer = new T[blocksize];
    _oscillatorBuffer = new float[blocksize];
    HLog("Allocated 2 X %d as local buffers", blocksize * sizeof(T));

    HLog("Create local oscilator");
    _localOscillator = new HLocalOscillator<float>(rate, frequency);
}

template <class T>
HMultiplier<T>::~HMultiplier()
{
    HLog("~HMultiplier()");
    delete _buffer;
    delete _oscillatorBuffer;
    delete _localOscillator;
}

template <class T>
int HMultiplier<T>::Read(T* dest, size_t blocksize)
{
    if( blocksize > _blocksize )
    {
        HError("Illegal blocksize in Read() to HMultiplier. Initialized with %d called with %d", _blocksize, blocksize);
        throw new HFilterIOException("It is not possible to read more data than the size given at creation of the multiplier");
    }

    // Read from input
    int received = _reader->Read(_buffer, blocksize);
    if( received <= 0)
    {
        HLog("Zero read from reader, stopping");
        return 0;
    }

    // Read localoscillator signal
    _localOscillator->Read(_oscillatorBuffer, blocksize);

    // Multiply inputs (= convolution in freq. domain = frequency shift)
    for( int i = 0; i < blocksize; i++ )
    {
        //float y = 2 * M_PI * (1000.0 / 48000.0) * n;

        //float s = _buffer_1[i] * cos(y);

        //Complex e(cos(y), sin(y));
        //std::cout << "e n y " << e.real() << " " << n << " " << y << std::endl;
        //Complex q = e * x1[i];
        // float s = q.real();

        //float s = (float) _buffer_1[i] * ((float) _buffer_2[i] / 5000);
        //std::cout << "buffer cos " << _buffer_2[i] << " " << cos(y) << std::endl;;


        dest[i] = _buffer[i] * _oscillatorBuffer[i];
        //dest[i] = _buffer_1[i] * _buffer_2[i];
        //dest[i] = (x1[i] * x2[i]).real();
        //dest[i] = x2[i].real();
        //dest[i] = _buffer_2[i];
    }
    return blocksize;
}

template <class T>
bool HMultiplier<T>::Start()
{
    HLog("Calling Start() on reader");
    return _reader->Start();
}

template <class T>
bool HMultiplier<T>::Stop()
{
    HLog("Calling Stop() on reader");
    return _reader->Stop();
}

/********************************************************************
Explicit instantiation
********************************************************************/

// HMultiplier
template
HMultiplier<int8_t>::HMultiplier(HReader<int8_t>* reader_1, H_SAMPLE_RATE rate, int frequency, size_t blocksize);

template
HMultiplier<uint8_t>::HMultiplier(HReader<uint8_t>* reader_1, H_SAMPLE_RATE rate, int frequency, size_t blocksize);

template
HMultiplier<int16_t>::HMultiplier(HReader<int16_t>* reader_1, H_SAMPLE_RATE rate, int frequency, size_t blocksize);

template
HMultiplier<int32_t>::HMultiplier(HReader<int32_t>* reader_1, H_SAMPLE_RATE rate, int frequency, size_t blocksize);

// ~HMultiplier()
template
HMultiplier<int8_t>::~HMultiplier();

template
HMultiplier<uint8_t>::~HMultiplier();

template
HMultiplier<int16_t>::~HMultiplier();

template
HMultiplier<int32_t>::~HMultiplier();

// Read()
template
int HMultiplier<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HMultiplier<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HMultiplier<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HMultiplier<int32_t>::Read(int32_t* dest, size_t blocksize);

// Start()
template
bool HMultiplier<int8_t>::Start();

template
bool HMultiplier<uint8_t>::Start();

template
bool HMultiplier<int16_t>::Start();

template
bool HMultiplier<int32_t>::Start();

// Stop()
template
bool HMultiplier<int8_t>::Stop();

template
bool HMultiplier<uint8_t>::Stop();

template
bool HMultiplier<int16_t>::Stop();

template
bool HMultiplier<int32_t>::Stop();

#endif