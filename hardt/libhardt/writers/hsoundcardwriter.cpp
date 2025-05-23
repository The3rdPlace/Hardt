#ifndef __HSoundcardWriter_CPP
#define __HSoundcardWriter_CPP

#include <stdio.h>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <cstring>

#include "hsoundcardwriter.h"

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HSoundcardWriter<T>::HSoundcardWriter(std::string id, int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer):
    HWriter<T>(id),
    _isInitialized(false),
    _isStarted(false)
{
    HLog("HSoundcardWriter(device=%d, rate=%d, channels=%d, format=%d, framesPerBuffer=%d)", device, rate, channels, format, framesPerBuffer);
    Init(device, rate, channels, format, framesPerBuffer);
}

template <class T>
HSoundcardWriter<T>::HSoundcardWriter(std::string id, int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer, HWriterConsumer<T>* consumer):
    HWriter<T>(id),
    _isInitialized(false),
    _isStarted(false)
{
    HLog("HSoundcardWriter(device=%d, rate=%d, channels=%d, format=%d, framesPerBuffer=%d)", device, rate, channels, format, framesPerBuffer);
    Init(device, rate, channels, format, framesPerBuffer);

    consumer->SetWriter(this);
}

template <class T>
void HSoundcardWriter<T>::Init(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer)
{
    // Initialize resources used by the callback function
    _cbd.channels = channels;
    _cbd.framesize = framesPerBuffer;
    _cbd.wrloc = 0;
    _cbd.rdloc = 0;
    _cbd.buffer = new T[NUMBER_OF_BUFFERS * framesPerBuffer * channels];
    if( _cbd.buffer == NULL )
    {
        HError("Unable to allocate %d buffers for %d frames á %d bytes for %d channel(s)", NUMBER_OF_BUFFERS, framesPerBuffer, sizeof(T), channels);
        throw new HInitializationException("Out of memory when allocating buffers");
    }
    HLog("Buffers allocated");

    // Initialize buffers so that we have total silence untill something is written to the buffers
    memset((void*) _cbd.buffer, std::numeric_limits<T>::is_signed ? 0 : std::numeric_limits<T>::max() / 2, NUMBER_OF_BUFFERS * framesPerBuffer * channels * sizeof(T));
    HLog("Initialized soundcard buffers with value %d", std::numeric_limits<T>::is_signed ? 0 : std::numeric_limits<T>::max() / 2);

    PaError err = Pa_Initialize();
	if( err != paNoError )
	{
		HError("Pa_Initialize() error: %s", Pa_GetErrorText(err));
		throw new HInitializationException(Pa_GetErrorText(err));
	}
    HLog("PortAudio initialized");

    // Setup output parameters
    PaStreamParameters outputParameters;
    outputParameters.device = device;
    outputParameters.channelCount = channels;
    outputParameters.sampleFormat = HSoundcard::GetPortAudioFormat(format);
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(device)->defaultLowInputLatency ;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    // Get an input stream
    PaStreamFlags flags = paNoFlag;
    err = Pa_OpenStream(&_stream, NULL, &outputParameters,
        rate,
        framesPerBuffer,
        flags,
        callback,
        &_cbd);

    if( err != paNoError )
    {
        HError("Pa_OpenStream() error: %s", Pa_GetErrorText(err));
    	throw new HInitializationException(Pa_GetErrorText(err));
    }
    HLog("Got output stream");

    // Ready
    _isInitialized = true;
}

template <class T>
HSoundcardWriter<T>::~HSoundcardWriter()
{
    HLog("~HSoundcardWriter()");

    if( _isInitialized )
    {
        // Make sure the stream is stopped
        Stop();
        HLog("Stream is stopped");

        // Terminate PortAudio
        PaError err = Pa_Terminate();
        if ( err != paNoError )
        {
            HError("Could not terminate PortAudio: %s", Pa_GetErrorText(err));
        }
        else
        {
            HLog("PortAudio terminated");
        }

        // Delete shared resources
        delete _cbd.buffer;
        HLog("Buffers released");
    }
}

template <class T>
int HSoundcardWriter<T>::WriteImpl(T* src, size_t blocksize)
{
    // Requested blocksize can not be larger than the device blocksize
    // (actively preventing large writes that would inhibit performance and
    // responsiveness when working with a synchroneous device such as a soundcard)
    if( blocksize != _cbd.framesize * _cbd.channels )
    {
        Stop();
        HError("It is not allowed to write more data than what the card ships per synchroneous callback");
        throw new HAudioIOException("It is not allowed to write more data than what the card ships per synchroneous callback");
    }

    // Make sure we are running
    if( !_isStarted )
    {
        HError("Stream is not started, no data to write");
        throw new HAudioIOException("Stream is not started, no data to read");
    }

    // If read and write position is the same (same buffer), then wait untill a sample has been taken by the soundcard
    if( _cbd.wrloc == _cbd.rdloc )
    {
        std::unique_lock<std::mutex> lck(_cbd.mtx);
        _cbd.lock.wait(lck);
    }

    // Copy bytes to the next output buffer
    T* ptr = &_cbd.buffer[_cbd.wrloc];
    memcpy((void*) ptr, (void*) src, _cbd.framesize * _cbd.channels * sizeof(T));

    // Advance write position to next buffer, if we have written the last buffer,
    // then wrap around to the first buffer.
    _cbd.wrloc += _cbd.framesize * _cbd.channels;
    if( _cbd.wrloc >= (NUMBER_OF_BUFFERS * _cbd.framesize * _cbd.channels) )
    {
        _cbd.wrloc = 0;
    }

    // We always writes the entire buffer as given
    return _cbd.framesize * _cbd.channels;
}

template <class T>
int HSoundcardWriter<T>::callback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    // Cast data passed through stream to our structure.
    CallbackData *data = (CallbackData*) userData;

    // Cast input- and outputbuffers to our specific data type
    T* src = (T*) &data->buffer[data->rdloc];
    T* dest = (T*) outputBuffer;

    // Copy new data from the buffer to the soundcard
    memcpy((void*) dest, (void*) src, framesPerBuffer * data->channels * sizeof(T));

    // Advance write position to next buffer, if we have read the last buffer,
    // then wrap around to the first buffer.
    data->rdloc += data->framesize * data->channels;
    if( data->rdloc >= (NUMBER_OF_BUFFERS * data->framesize * data->channels) )
    {
        data->rdloc = 0;
    }

    // We might have a waiting writer, signal that data is available
    // Since we are not waiting on the mutex, using it here should be fine.
    std::unique_lock<std::mutex> lck(data->mtx);
    data->lock.notify_one();

    // Done, continue sampling
    return paContinue;
}

template <class T>
bool HSoundcardWriter<T>::Start()
{
    if( !_isInitialized )
    {
        HLog("Refusing to start a stream when not initialized");
        return false;
    }

    HLog("Starting output stream");
    PaError err = Pa_StartStream( _stream );
    if( err != paNoError )
    {
        HError("Could not start input stream: %s", Pa_GetErrorText(err));
        return false;
    }
    _isStarted = true;
    HLog("Output stream started");
    return true;
}

template <class T>
bool HSoundcardWriter<T>::Stop()
{
    if( !_isInitialized )
    {
        HLog("Refusing to stop a stream when not initialized");
        return false;
    }

    HLog("Stopping input stream");
    if( !Pa_IsStreamStopped( _stream ) )
    {
        PaError err = Pa_StopStream( _stream );
        if( err != paNoError )
        {
            HError("Could not stop output stream: %s", Pa_GetErrorText(err));
            return false;
        }
        _isStarted = false;
        HLog("Output stream stopped");
    }
    else
    {
        HLog("Stream is already stopped");
    }
    return true;
}


/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HSoundcardWriter()
template
HSoundcardWriter<int8_t>::HSoundcardWriter(std::string id, int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer);

template
HSoundcardWriter<uint8_t>::HSoundcardWriter(std::string id, int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer);

template
HSoundcardWriter<int16_t>::HSoundcardWriter(std::string id, int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer);

template
HSoundcardWriter<int32_t>::HSoundcardWriter(std::string id, int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer);

template
HSoundcardWriter<int8_t>::HSoundcardWriter(std::string id, int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer, HWriterConsumer<int8_t>* consumer);

template
HSoundcardWriter<uint8_t>::HSoundcardWriter(std::string id, int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer, HWriterConsumer<uint8_t>* consumer);

template
HSoundcardWriter<int16_t>::HSoundcardWriter(std::string id, int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer, HWriterConsumer<int16_t>* consumer);

template
HSoundcardWriter<int32_t>::HSoundcardWriter(std::string id, int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer, HWriterConsumer<int32_t>* consumer);

// ~HSoundcardWriter()
template
HSoundcardWriter<int8_t>::~HSoundcardWriter();

template
HSoundcardWriter<uint8_t>::~HSoundcardWriter();

template
HSoundcardWriter<int16_t>::~HSoundcardWriter();

template
HSoundcardWriter<int32_t>::~HSoundcardWriter();

// Read()
template
int HSoundcardWriter<int8_t>::WriteImpl(int8_t* dest, size_t blocksize);

template
int HSoundcardWriter<uint8_t>::WriteImpl(uint8_t* dest, size_t blocksize);

template
int HSoundcardWriter<int16_t>::WriteImpl(int16_t* dest, size_t blocksize);

template
int HSoundcardWriter<int32_t>::WriteImpl(int32_t* dest, size_t blocksize);

//! @endcond
#endif
