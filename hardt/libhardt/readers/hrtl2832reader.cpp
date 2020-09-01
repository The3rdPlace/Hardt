#ifndef __HRTL2832READER_CPP
#define __HRTL2832READER_CPP

#include <stdio.h>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <cstring>

#include "hrtl2832reader.h"

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HRtl2832Reader<T>::HRtl2832Reader(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer):
        _isInitialized(false),
        _isStarted(false)
{
    HLog("HRtl2832Reader(device=%d, rate=%d, channels=%d, format=%d, framesPerBuffer=%d)", device, rate, channels, format, framesPerBuffer);

    // Initialize resources used by the callback function
    _cbd.channels = channels;
    _cbd.framesize = framesPerBuffer;
    _cbd.wrloc = 0;
    _cbd.rdloc = 0;
    _cbd.buffer = new T[NUMBER_OF_BUFFERS * framesPerBuffer];
    if( _cbd.buffer == NULL )
    {
        HError("Unable to allocate %d buffers for %d frames á %d bytes", NUMBER_OF_BUFFERS, framesPerBuffer, sizeof(T));
        throw new HInitializationException("Out of memory when allocating buffers");
    }
    HLog("Buffers allocated");

    /*PaError err = Pa_Initialize();
    if( err != paNoError )
    {
        HError("Pa_Initialize() error: %s", Pa_GetErrorText(err));
        throw new HInitializationException(Pa_GetErrorText(err));
    }
    HLog("PortAudio initialized");

    // Setup input parameters
    PaStreamParameters inputParameters;
    inputParameters.device = device;
    inputParameters.channelCount = channels;
    inputParameters.sampleFormat = HSoundcard::GetPortAudioFormat(format);
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(device)->defaultLowInputLatency ;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    // Get an input stream
    PaStreamFlags flags = paNoFlag;
    err = Pa_OpenStream(&_stream, &inputParameters, NULL,
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
    HLog("Got input stream");
*/
    // Ready
    _isInitialized = true;
}

template <class T>
HRtl2832Reader<T>::~HRtl2832Reader()
{
    HLog("~HRtl2832Reader()");

    if( _isInitialized )
    {
        // Make sure the stream is stopped
        Stop();
        HLog("Stream is stopped");

        // Terminate PortAudio
        /*PaError err = Pa_Terminate();
        if ( err != paNoError )
        {
            HError("Could not terminate PortAudio: %s", Pa_GetErrorText(err));
        }
        else
        {
            HLog("PortAudio terminated");
        }*/

        // Delete shared resources
        delete _cbd.buffer;
        HLog("Buffers released");
    }
}

template <class T>
int HRtl2832Reader<T>::Read(T* dest, size_t blocksize)
{
    // Requested blocksize can not be larger than the device blocksize
    // (actively preventing large reads that would inhibit performance and
    // responsiveness when working with a synchroneous device such as a soundcard)
    if( blocksize > _cbd.framesize )
    {
        Stop();
        throw new HAudioIOException("It is not allowed to read  more data than what the card ships per synchroneous callback");
    }

    // Make sure we are running
    if( !_isStarted )
    {
        throw new HAudioIOException("Stream is not started, no data to read");
    }

    // If read and write position is the same (same buffer), then wait for new samples
    if( _cbd.wrloc == _cbd.rdloc )
    {
        std::unique_lock<std::mutex> lck(_cbd.mtx);
        _cbd.lock.wait(lck);
    }

    // If we have samples available, then read the next buffer
    if( _cbd.wrloc != _cbd.rdloc )
    {
        T* ptr = &_cbd.buffer[_cbd.rdloc];
        memcpy((void*) dest, (void*) ptr, _cbd.framesize * sizeof(T));

        // Advance read position to next buffer, if we have read the last buffer,
        // then wrap around to the first buffer.
        _cbd.rdloc += _cbd.framesize;
        if( _cbd.rdloc >= (NUMBER_OF_BUFFERS * _cbd.framesize) )
        {
            _cbd.rdloc = 0;
        }

        // We always reads the entire buffer as given
        return _cbd.framesize;
    }

    // No new data available, return zero write.
    // Most likely, this indicates that we are shutting down (unclean termination)
    HLog("No data available for read()");
    return 0;
}

/*
template <class T>
int HRtl2832Reader<T>::callback( const void *inputBuffer, void *outputBuffer,
                                   unsigned long framesPerBuffer,
                                   const PaStreamCallbackTimeInfo* timeInfo,
                                   PaStreamCallbackFlags statusFlags,
                                   void *userData )
{
    // Cast data passed through stream to our structure.
    CallbackData *data = (CallbackData*) userData;

    // Cast input- and outputbuffers to our specific data type
    T* src = (T*) inputBuffer;
    T* dest = (T*) &data->buffer[data->wrloc];

    // Copy new data from the soundcard to the buffer
    memcpy((void*) dest, (void*) src, framesPerBuffer * sizeof(T));

    // Advance write position to next buffer, if we have written the last buffer,
    // then wrap around to the first buffer.
    data->wrloc += data->framesize;
    if( data->wrloc >= (NUMBER_OF_BUFFERS * data->framesize) )
    {
        data->wrloc = 0;
    }

    // We might have a waiting reader, signal that data is available
    // Since we are not waiting on the mutex, using it here should be fine.
    std::unique_lock<std::mutex> lck(data->mtx);
    data->lock.notify_one();

    // Done, continue sampling
    return paContinue;
}
*/

template <class T>
bool HRtl2832Reader<T>::Start()
{
    if( !_isInitialized )
    {
        HLog("Refusing to start a stream when not initialized");
        return false;
    }

    HLog("Starting input stream");
    /*PaError err = Pa_StartStream( _stream );
    if( err != paNoError )
    {
        HError("Could not start input stream: %s", Pa_GetErrorText(err));
        return false;
    }*/
    _isStarted = true;
    HLog("Input stream started");
    return true;
}

template <class T>
bool HRtl2832Reader<T>::Stop()
{
    if( !_isInitialized )
    {
        HLog("Refusing to stop a stream when not initialized");
        return false;
    }

    HLog("Stopping input stream");
    /*if( !Pa_IsStreamStopped( _stream ) )
    {
        PaError err = Pa_StopStream( _stream );
        if( err != paNoError )
        {
            HError("Could not stop input stream: %s", Pa_GetErrorText(err));
            return false;
        }
        _isStarted = false;
        HLog("Input stream stopped");
    }
    else
    {
        HLog("Stream is already stopped");
    }*/
    return true;
}


/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HRtl2832Reader()
template
HRtl2832Reader<int8_t>::HRtl2832Reader(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer = DEFAULT_FRAMESIZE);

template
HRtl2832Reader<uint8_t>::HRtl2832Reader(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer = DEFAULT_FRAMESIZE);

template
HRtl2832Reader<int16_t>::HRtl2832Reader(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer = DEFAULT_FRAMESIZE);

template
HRtl2832Reader<int32_t>::HRtl2832Reader(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer = DEFAULT_FRAMESIZE);

// ~HRtl2832Reader()
template
HRtl2832Reader<int8_t>::~HRtl2832Reader();

template
HRtl2832Reader<uint8_t>::~HRtl2832Reader();

template
HRtl2832Reader<int16_t>::~HRtl2832Reader();

template
HRtl2832Reader<int32_t>::~HRtl2832Reader();

// Read()
template
int HRtl2832Reader<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HRtl2832Reader<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HRtl2832Reader<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HRtl2832Reader<int32_t>::Read(int32_t* dest, size_t blocksize);

//! @endcond
#endif
