#ifndef __HIQMULTIPLIER_CPP
#define __HIQMULTIPLIER_CPP

#include "hreader.h"
#include "hiqmultiplier.h"

template <class T>
HIqMultiplier<T>::HIqMultiplier(HReader<T>* reader, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<T>* probe):
    _reader(reader),
    _writer(NULL),
    _blocksize(blocksize),
    _probe(probe)
{
    HLog("HIqMultiplier(HReader*, %d, %d, %d)", rate, frequency, blocksize);
    Init(rate, frequency, oscillatorAmplitude, blocksize);
}

template <class T>
HIqMultiplier<T>::HIqMultiplier(HWriter<T>* writer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<T>* probe):
    _reader(NULL),
    _writer(writer),
    _blocksize(blocksize),
    _probe(probe)
{
    HLog("HIqMultiplier(HWriter*, %d, %d, %d)", rate, frequency, blocksize);
    Init(rate, frequency, oscillatorAmplitude, blocksize);
}

template <class T>
HIqMultiplier<T>::HIqMultiplier(HWriterConsumer<T>* consumer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<T>* probe):
    _reader(NULL),
    _writer(NULL),
    _blocksize(blocksize),
    _probe(probe)
{
    HLog("HIqMultiplier(HWriterConsumer*, %d, %d, %d)", rate, frequency, blocksize);
    Init(rate, frequency, oscillatorAmplitude, blocksize);

    consumer->SetWriter(this);
    HLog("Registered as writer with previous writer");
}

template <class T>
HIqMultiplier<T>::~HIqMultiplier()
{
    HLog("~HIqMultiplier()");
    delete _buffer;
    delete _oscillatorSinBuffer;
    delete _oscillatorCosBuffer;
    delete _localSinOscillator;
    delete _localCosOscillator;
}

template <class T>
void HIqMultiplier<T>::Init(H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize)
{
    _buffer = new T[blocksize];
    _oscillatorSinBuffer = new T[blocksize / 2];
    _oscillatorCosBuffer = new T[blocksize / 2];
    HLog("Allocated local buffers");

    _localSinOscillator = new HLocalOscillator<T>(rate, frequency, oscillatorAmplitude);
    _localCosOscillator = new HLocalOscillator<T>(rate, frequency, oscillatorAmplitude, 0);
    HLog("Created local oscilators");
}

template <class T>
int HIqMultiplier<T>::Read(T* dest, size_t blocksize)
{
    if( blocksize > _blocksize )
    {
        HError("Illegal blocksize in Read() to HIqMultiplier. Initialized with %d called with %d", _blocksize, blocksize);
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

    if( _probe != NULL )
    {
        _probe->Write(dest, blocksize);
    }

    return blocksize;
}

template <class T>
int HIqMultiplier<T>::Write(T* src, size_t blocksize)
{
    if( blocksize > _blocksize )
    {
        HError("Illegal blocksize in Write() to HIqMultiplier. Initialized with %d called with %d", _blocksize, blocksize);
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

    if( _probe != NULL )
    {
        _probe->Write(_buffer, blocksize);
    }

    return blocksize;
}

template <class T>
void HIqMultiplier<T>::Mix(T* src, T* dest, size_t blocksize)
{
    // Read localoscillator signal and convert to complex signal
    _localSinOscillator->Read(_oscillatorSinBuffer, blocksize / 2);
    _localCosOscillator->Read(_oscillatorCosBuffer, blocksize / 2);

    // Multiply inputs (= convolution in freq. domain = frequency shift)
    for( int i = 0; i < blocksize; i += 2 )
    {
        T rSrc = src[i];
        T jSrc = src[i + 1];
        T rOsc = _oscillatorSinBuffer[i / 2];
        T jOsc = _oscillatorCosBuffer[i / 2];

        T R = (rSrc * rOsc) + (jSrc * jOsc);
        T J = ((rSrc + jSrc) * (rOsc + jOsc)) - (rSrc * rOsc) - (jSrc * jOsc);

        dest[i] = R;
        dest[i + 1] = J;
    }
}

template <class T>
bool HIqMultiplier<T>::Start()
{
    if( _reader != NULL ) {
        HLog("Calling Start() on reader");
        return _reader->Start();
    }
    if( _writer != NULL ) {
        HLog("Calling Start() on writer");
        return _writer->Start();
    }
    return false;
}

template <class T>
bool HIqMultiplier<T>::Stop()
{
    if( _reader != NULL ) {
        HLog("Calling Stop() on reader");
        return _reader->Stop();
    }
    if( _writer != NULL ) {
        HLog("Calling Stop() on writer");
        return _writer->Stop();
    }
    return false;
}

template <class T>
void HIqMultiplier<T>::SetFrequency(int frequency)
{
    HLog("Setting frequency = %d", frequency);
    _localSinOscillator->SetFrequency(frequency);
    _localCosOscillator->SetFrequency(frequency);
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HIqMultiplier
template
HIqMultiplier<int8_t>::HIqMultiplier(HReader<int8_t>* reader_1, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<int8_t>* probe);

template
HIqMultiplier<uint8_t>::HIqMultiplier(HReader<uint8_t>* reader_1, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<uint8_t>* probe);

template
HIqMultiplier<int16_t>::HIqMultiplier(HReader<int16_t>* reader_1, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<int16_t>* probe);

template
HIqMultiplier<int32_t>::HIqMultiplier(HReader<int32_t>* reader_1, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<int32_t>* probe);

template
HIqMultiplier<int8_t>::HIqMultiplier(HWriter<int8_t>* writer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<int8_t>* probe);

template
HIqMultiplier<uint8_t>::HIqMultiplier(HWriter<uint8_t>* writer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<uint8_t>* probe);

template
HIqMultiplier<int16_t>::HIqMultiplier(HWriter<int16_t>* writer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<int16_t>* probe);

template
HIqMultiplier<int32_t>::HIqMultiplier(HWriter<int32_t>* writer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<int32_t>* probe);

template
HIqMultiplier<int8_t>::HIqMultiplier(HWriterConsumer<int8_t>* consumer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<int8_t>* probe);

template
HIqMultiplier<uint8_t>::HIqMultiplier(HWriterConsumer<uint8_t>* consumer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<uint8_t>* probe);

template
HIqMultiplier<int16_t>::HIqMultiplier(HWriterConsumer<int16_t>* consumer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<int16_t>* probe);

template
HIqMultiplier<int32_t>::HIqMultiplier(HWriterConsumer<int32_t>* consumer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<int32_t>* probe);

// ~HIqMultiplier()
template
HIqMultiplier<int8_t>::~HIqMultiplier();

template
HIqMultiplier<uint8_t>::~HIqMultiplier();

template
HIqMultiplier<int16_t>::~HIqMultiplier();

template
HIqMultiplier<int32_t>::~HIqMultiplier();

// Init()
template
void HIqMultiplier<int8_t>::Init(H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize);

template
void HIqMultiplier<uint8_t>::Init(H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize);

template
void HIqMultiplier<int16_t>::Init(H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize);

template
void HIqMultiplier<int32_t>::Init(H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize);

// Read()
template
int HIqMultiplier<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HIqMultiplier<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HIqMultiplier<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HIqMultiplier<int32_t>::Read(int32_t* dest, size_t blocksize);

// Write()
template
int HIqMultiplier<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HIqMultiplier<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HIqMultiplier<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HIqMultiplier<int32_t>::Write(int32_t* src, size_t blocksize);

// Start()
template
bool HIqMultiplier<int8_t>::Start();

template
bool HIqMultiplier<uint8_t>::Start();

template
bool HIqMultiplier<int16_t>::Start();

template
bool HIqMultiplier<int32_t>::Start();

// Stop()
template
bool HIqMultiplier<int8_t>::Stop();

template
bool HIqMultiplier<uint8_t>::Stop();

template
bool HIqMultiplier<int16_t>::Stop();

template
bool HIqMultiplier<int32_t>::Stop();

// SetFrequency
template
void HIqMultiplier<int8_t>::SetFrequency(int frequency);

template
void HIqMultiplier<uint8_t>::SetFrequency(int frequency);

template
void HIqMultiplier<int16_t>::SetFrequency(int frequency);

template
void HIqMultiplier<int32_t>::SetFrequency(int frequency);

// Mix()
template
void HIqMultiplier<int8_t>::Mix(int8_t* src, int8_t* dest, size_t blocksize);

template
void HIqMultiplier<uint8_t>::Mix(uint8_t* src, uint8_t* dest, size_t blocksize);

template
void HIqMultiplier<int16_t>::Mix(int16_t* src, int16_t* dest, size_t blocksize);

template
void HIqMultiplier<int32_t>::Mix(int32_t* src, int32_t* dest, size_t blocksize);

//! @endcond
#endif