#ifndef __HSOUNDCARDREADER_H
#define __HSOUNDCARDREADER_H

#define NUMBER_OF_BUFFERS 4
#define DEFAULT_FRAMESIZE 1024

#include <stdio.h>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <cstring>

// Supported PortAudio sample rates, typed to hide portaudio specific names
#define H_SAMPLE_FORMAT PaSampleFormat
#define H_SAMPLE_FORMAT_INT_8 paInt8
#define H_SAMPLE_FORMAT_UINT_8 paUInt8
#define H_SAMPLE_FORMAT_INT_16 paInt16
//#define H_SAMPLE_FORMAT_INT_24 paInt24  // Need some extra work in the reader before we can enable this
#define H_SAMPLE_FORMAT_INT_32 paInt32

// Samplerates. Not all rates are supported by any given card
#define H_SAMPLE_RATE int
#define H_SAMPLE_RATE_8K 8000
#define H_SAMPLE_RATE_11K 11025
#define H_SAMPLE_RATE_22K 22050
#define H_SAMPLE_RATE_32K 32000
#define H_SAMPLE_RATE_44K1 44100
#define H_SAMPLE_RATE_48K 48000
#define H_SAMPLE_RATE_96K 96000
#define H_SAMPLE_RATE_192K 192000

template <class T>
class HSoundcardReader : public HReader<T>
{
    private:

        // Data accessed by the (static) callback function
        struct CallbackData
        {
            int channels;
            int framesize;
            int wrloc;
            int rdloc;
            T* buffer;
            bool firstCall;
            std::mutex mtx;
            std::condition_variable lock;
        } _cbd;

        // The sample stream from the soundcard
        PaStream *_stream;

        // Housekeeping
        bool _isInitialized;

        bool Start(void* unused);
        bool Stop();

    public:

        HSoundcardReader(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer = DEFAULT_FRAMESIZE);
        ~HSoundcardReader();
        int Read(T* dest);
        int Blocksize();

        static int callback( const void *inputBuffer, void *outputBuffer,
                                   unsigned long framesPerBuffer,
                                   const PaStreamCallbackTimeInfo* timeInfo,
                                   PaStreamCallbackFlags statusFlags,
                                   void *userData );
};

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HSoundcardReader<T>::HSoundcardReader(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer):
    _isInitialized(false)
{
    HLog("HSoundCardReader(device=%d, rate=%d, channels=%d, format=%d, framesPerBuffer=%d)", device, rate, channels, format, framesPerBuffer);

    // Initialize resources used by the callback function
    _cbd.channels = channels;
    _cbd.framesize = framesPerBuffer;
    _cbd.wrloc = 0;
    _cbd.rdloc = 0;
    _cbd.firstCall = true;
    _cbd.buffer = new T[NUMBER_OF_BUFFERS * framesPerBuffer * sizeof(T)];
    if( _cbd.buffer == NULL )
    {
        HError("Unable to allocate %d buffers for %d frames á %d bytes", NUMBER_OF_BUFFERS, framesPerBuffer, sizeof(T));
        throw new HInitializationException("Out of memory when allocating buffers");
    }
    HLog("Buffers allocated");

    PaError err = Pa_Initialize();
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
    inputParameters.sampleFormat = format;
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

    // Ready
    _isInitialized = true;
}

template <class T>
HSoundcardReader<T>::~HSoundcardReader()
{
    HLog("~HSoundcardReader()");

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
int HSoundcardReader<T>::Blocksize()
{
    return _cbd.framesize;
}

template <class T>
int HSoundcardReader<T>::Read(T* dest)
{
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

        // We always reads the entire buffer
        return _cbd.framesize;
    }

    // No new data available, return zero write.
    // Most likely, this indicates that we are shutting down (unclean termination)
    HLog("No data available for read()");
    return 0;
}

template <class T>
int HSoundcardReader<T>::callback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    // Cast data passed through stream to our structure.
    CallbackData *data = (CallbackData*) userData;

    // Usually, you do not want any logging from this function!!
    // however - it could be usefull to be notified that the function
    // gets called, so we log the very first call after starting the stream
    if( data->firstCall )
    {
        HLog("Received %ul frames from the soundcard", framesPerBuffer);
        data->firstCall = false;
    }

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
    // Since we are not waiting on the mutext, using it here should be fine.
    std::unique_lock<std::mutex> lck(data->mtx);
    data->lock.notify_one();

    // Done, continue sampling
    return paContinue;
}

template <class T>
bool HSoundcardReader<T>::Start(void* unused)
{
    if( !_isInitialized )
    {
        HLog("Refusing to start a stream when not initialized");
        return false;
    }

    HLog("Starting input stream");
    _cbd.firstCall = true;
    PaError err = Pa_StartStream( _stream );
    if( err != paNoError )
    {
        HError("Could not start input stream: %s", Pa_GetErrorText(err));
        return false;
    }
    HLog("Input stream started");
    return true;
}

template <class T>
bool HSoundcardReader<T>::Stop()
{
    if( !_isInitialized )
    {
        HLog("Refusing to stop a stream when not initialized");
        return false;
    }

    HLog("Stopping input stream");
    PaError err = Pa_StopStream( _stream );
    if( err != paNoError )
    {
        HError("Could not stop input stream: %s", Pa_GetErrorText(err));
        return false;
    }
    HLog("Input stream stopped");
    return true;
}

#endif
