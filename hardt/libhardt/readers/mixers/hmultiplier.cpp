#ifndef __HMultiplier_CPP
#define __HMultiplier_CPP

#include "hreader.h"
#include "hmultiplier.h"

template <class T>
HMultiplier<T>::HMultiplier(HReader<T>* reader, H_SAMPLE_RATE rate, int frequency, size_t blocksize):
    _reader(reader),
    _writer(NULL),
    _blocksize(blocksize)
{
    HLog("HMultiplier(HReader*, %d, %d, %d)", rate, frequency, blocksize);
    Init(rate, frequency, blocksize);
}

template <class T>
HMultiplier<T>::HMultiplier(HWriter<T>* writer, H_SAMPLE_RATE rate, int frequency, size_t blocksize):
    _reader(NULL),
    _writer(writer),
    _blocksize(blocksize)
{
    HLog("HMultiplier(HWriter*, %d, %d, %d)", rate, frequency, blocksize);
    Init(rate, frequency, blocksize);
}

template <class T>
HMultiplier<T>::HMultiplier(HWriterConsumer<T>* consumer, H_SAMPLE_RATE rate, int frequency, size_t blocksize):
    _reader(NULL),
    _writer(NULL),
    _blocksize(blocksize)
{
    HLog("HMultiplier(HWriter*, %d, %d, %d)", rate, frequency, blocksize);
    Init(rate, frequency, blocksize);

    consumer->SetWriter(this);
    HLog("Registered as writer with previous writer");
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
void HMultiplier<T>::Init(H_SAMPLE_RATE rate, int frequency, size_t blocksize)
{
    _buffer = new T[blocksize];
    _oscillatorBuffer = new float[blocksize];
    HLog("Allocated 2 X %d as local buffers", blocksize * sizeof(T));

    _localOscillator = new HLocalOscillator<float>(rate, frequency);
    HLog("Create local oscilator");
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

    // Mix signals
    Mix(_buffer, dest, blocksize);
    return blocksize;
}

template <class T>
int HMultiplier<T>::Write(T* src, size_t blocksize)
{
    if( blocksize > _blocksize )
    {
        HError("Illegal blocksize in Write() to HMultiplier. Initialized with %d called with %d", _blocksize, blocksize);
        throw new HFilterIOException("It is not possible to write more data than the size given at creation of the multiplier");
    }

    // Mix signals
    Mix(src, _buffer, blocksize);

    // Write to next writer
    int written = _writer->Write(_buffer, blocksize);
    if( written <= 0)
    {
        HLog("Zero write to writer, stopping");
        return 0;
    }
    return blocksize;
}

template <class T>
void HMultiplier<T>::Mix(T* src, T* dest, size_t blocksize)
{
    // Read localoscillator signal
    _localOscillator->Read(_oscillatorBuffer, blocksize);

    // Multiply inputs (= convolution in freq. domain = frequency shift)
    for( int i = 0; i < blocksize; i++ )
    {
        dest[i] = src[i] * _oscillatorBuffer[i];
    }
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

template <class T>
void HMultiplier<T>::SetFrequency(int frequency)
{
    HLog("Setting frequency = %d", frequency);
    _localOscillator->SetFrequency(frequency);
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HMultiplier
template
HMultiplier<int8_t>::HMultiplier(HReader<int8_t>* reader_1, H_SAMPLE_RATE rate, int frequency, size_t blocksize);

template
HMultiplier<uint8_t>::HMultiplier(HReader<uint8_t>* reader_1, H_SAMPLE_RATE rate, int frequency, size_t blocksize);

template
HMultiplier<int16_t>::HMultiplier(HReader<int16_t>* reader_1, H_SAMPLE_RATE rate, int frequency, size_t blocksize);

template
HMultiplier<int32_t>::HMultiplier(HReader<int32_t>* reader_1, H_SAMPLE_RATE rate, int frequency, size_t blocksize);

template
HMultiplier<int8_t>::HMultiplier(HWriter<int8_t>* writer, H_SAMPLE_RATE rate, int frequency, size_t blocksize);

template
HMultiplier<uint8_t>::HMultiplier(HWriter<uint8_t>* writer, H_SAMPLE_RATE rate, int frequency, size_t blocksize);

template
HMultiplier<int16_t>::HMultiplier(HWriter<int16_t>* writer, H_SAMPLE_RATE rate, int frequency, size_t blocksize);

template
HMultiplier<int32_t>::HMultiplier(HWriter<int32_t>* writer, H_SAMPLE_RATE rate, int frequency, size_t blocksize);

template
HMultiplier<int8_t>::HMultiplier(HWriterConsumer<int8_t>* consumer, H_SAMPLE_RATE rate, int frequency, size_t blocksize);

template
HMultiplier<uint8_t>::HMultiplier(HWriterConsumer<uint8_t>* consumer, H_SAMPLE_RATE rate, int frequency, size_t blocksize);

template
HMultiplier<int16_t>::HMultiplier(HWriterConsumer<int16_t>* consumer, H_SAMPLE_RATE rate, int frequency, size_t blocksize);

template
HMultiplier<int32_t>::HMultiplier(HWriterConsumer<int32_t>* consumer, H_SAMPLE_RATE rate, int frequency, size_t blocksize);

// ~HMultiplier()
template
HMultiplier<int8_t>::~HMultiplier();

template
HMultiplier<uint8_t>::~HMultiplier();

template
HMultiplier<int16_t>::~HMultiplier();

template
HMultiplier<int32_t>::~HMultiplier();

// Init()
template
void HMultiplier<int8_t>::Init(H_SAMPLE_RATE rate, int frequency, size_t blocksize);

template
void HMultiplier<uint8_t>::Init(H_SAMPLE_RATE rate, int frequency, size_t blocksize);

template
void HMultiplier<int16_t>::Init(H_SAMPLE_RATE rate, int frequency, size_t blocksize);

template
void HMultiplier<int32_t>::Init(H_SAMPLE_RATE rate, int frequency, size_t blocksize);

// Read()
template
int HMultiplier<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HMultiplier<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HMultiplier<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HMultiplier<int32_t>::Read(int32_t* dest, size_t blocksize);

// Write()
template
int HMultiplier<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HMultiplier<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HMultiplier<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HMultiplier<int32_t>::Write(int32_t* src, size_t blocksize);

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

// SetFrequency
template
void HMultiplier<int8_t>::SetFrequency(int frequency);

template
void HMultiplier<uint8_t>::SetFrequency(int frequency);

template
void HMultiplier<int16_t>::SetFrequency(int frequency);

template
void HMultiplier<int32_t>::SetFrequency(int frequency);

// Mix()
template
void HMultiplier<int8_t>::Mix(int8_t* src, int8_t* dest, size_t blocksize);

template
void HMultiplier<uint8_t>::Mix(uint8_t* src, uint8_t* dest, size_t blocksize);

template
void HMultiplier<int16_t>::Mix(int16_t* src, int16_t* dest, size_t blocksize);

template
void HMultiplier<int32_t>::Mix(int32_t* src, int32_t* dest, size_t blocksize);

//! @endcond
#endif