#ifndef __HMULTIPLIER_CPP
#define __HMULTIPLIER_CPP

#include "hreader.h"
#include "hmultiplier.h"

template <class T>
HMultiplier<T>::HMultiplier(std::string id, HReader<T>* reader, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<T>* probe):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _reader(reader),
    _writer(NULL),
    _blocksize(blocksize),
    _probe(probe),
    _scaling(oscillatorAmplitude)
{
    HLog("HMultiplier(HReader*, %d, %d, %d)", rate, frequency, blocksize);
    Init(rate, frequency, oscillatorAmplitude, blocksize);
}

template <class T>
HMultiplier<T>::HMultiplier(std::string id, HWriter<T>* writer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<T>* probe):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _reader(NULL),
    _writer(writer),
    _blocksize(blocksize),
    _probe(probe),
    _scaling(oscillatorAmplitude)
{
    HLog("HMultiplier(HWriter*, %d, %d, %d)", rate, frequency, blocksize);
    Init(rate, frequency, oscillatorAmplitude, blocksize);
}

template <class T>
HMultiplier<T>::HMultiplier(std::string id, HWriterConsumer<T>* consumer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<T>* probe):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _reader(NULL),
    _writer(NULL),
    _blocksize(blocksize),
    _probe(probe),
    _scaling(oscillatorAmplitude)
{
    HLog("HMultiplier(HWriterConsumer*, %d, %d, %d)", rate, frequency, blocksize);
    Init(rate, frequency, oscillatorAmplitude, blocksize);

    consumer->SetWriter(this);
    HLog("Registered as writer with previous writer");
}

template <class T>
HMultiplier<T>::~HMultiplier()
{
    HLog("~HMultiplier()");
    delete _buffer;
    delete _localOscillator;
}

template <class T>
void HMultiplier<T>::Init(H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize)
{
    _buffer = new T[blocksize];
    HLog("Allocated %d as local buffer", blocksize * sizeof(T));

    _localOscillator = new HLocalOscillator<T>(HWriter<T>::GetId(), rate, frequency, oscillatorAmplitude);
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

    if( _probe != NULL )
    {
        _probe->Write(dest, blocksize);
    }

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

    if( _probe != NULL )
    {
        _probe->Write(_buffer, blocksize);
    }

    return blocksize;
}

template <class T>
void HMultiplier<T>::Mix(T* src, T* dest, size_t blocksize)
{
    // Multiply inputs (= convolution in freq. domain = frequency shift)
    for( int i = 0; i < blocksize; i++ )
    {
        dest[i] = (T) (((long) src[i] * (long) _localOscillator->Next()) / (long) _scaling);
    }
}

template <class T>
bool HMultiplier<T>::Start()
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
bool HMultiplier<T>::Stop()
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
HMultiplier<int8_t>::HMultiplier(std::string id, HReader<int8_t>* reader_1, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<int8_t>* probe);

template
HMultiplier<uint8_t>::HMultiplier(std::string id, HReader<uint8_t>* reader_1, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<uint8_t>* probe);

template
HMultiplier<int16_t>::HMultiplier(std::string id, HReader<int16_t>* reader_1, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<int16_t>* probe);

template
HMultiplier<int32_t>::HMultiplier(std::string id, HReader<int32_t>* reader_1, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<int32_t>* probe);

template
HMultiplier<int8_t>::HMultiplier(std::string id, HWriter<int8_t>* writer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<int8_t>* probe);

template
HMultiplier<uint8_t>::HMultiplier(std::string id, HWriter<uint8_t>* writer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<uint8_t>* probe);

template
HMultiplier<int16_t>::HMultiplier(std::string id, HWriter<int16_t>* writer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<int16_t>* probe);

template
HMultiplier<int32_t>::HMultiplier(std::string id, HWriter<int32_t>* writer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<int32_t>* probe);

template
HMultiplier<int8_t>::HMultiplier(std::string id, HWriterConsumer<int8_t>* consumer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<int8_t>* probe);

template
HMultiplier<uint8_t>::HMultiplier(std::string id, HWriterConsumer<uint8_t>* consumer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<uint8_t>* probe);

template
HMultiplier<int16_t>::HMultiplier(std::string id, HWriterConsumer<int16_t>* consumer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<int16_t>* probe);

template
HMultiplier<int32_t>::HMultiplier(std::string id, HWriterConsumer<int32_t>* consumer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<int32_t>* probe);

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
void HMultiplier<int8_t>::Init(H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize);

template
void HMultiplier<uint8_t>::Init(H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize);

template
void HMultiplier<int16_t>::Init(H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize);

template
void HMultiplier<int32_t>::Init(H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize);

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