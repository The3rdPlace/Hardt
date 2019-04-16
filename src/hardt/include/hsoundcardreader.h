#ifndef __HSOUNDCARDREADER_H
#define __HSOUNDCARDREADER_H

#define NUMBER_OF_BUFFERS 4

#include <stdio.h>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <cstring>

template <class T>
class HSoundcardReader : public HReader<T>
{
    private:

        // Data accessed by the (static) callback function
        struct CallbackData
        {
            int framesize;
            int wrloc;
            int rdloc;
            T* buffer;
            std::mutex mtx;
            std::condition_variable cv;
        } _cbd;

        // The sample stream from the soundcard
        PaStream *_stream;

        // Housekeeping
        bool _isInitialized;

        bool Start();
        bool Stop();

    public:


        HSoundcardReader(int device, int rate, int channels, PaSampleFormat format, int framesPerBuffer);
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
HSoundcardReader<T>::HSoundcardReader(int device, int rate, int channels, PaSampleFormat format, int framesPerBuffer) :
    _isInitialized(false)
{
    HLog("Initializing");
    HError("This is an error");

    // Initialize resources used by the callback function
    _cbd.framesize = framesPerBuffer;
    _cbd.wrloc = 0;
    _cbd.rdloc = 0;
    _cbd.buffer = new T[NUMBER_OF_BUFFERS * framesPerBuffer * sizeof(T)];
    if( _cbd.buffer == NULL )
    {
        std::ostringstream msg;
        msg << "Unable to allocate 4 buffers for " << framesPerBuffer << " frames á " << sizeof(T) << " bytes";
        HError(msg.str());
    }



    PaError err = Pa_Initialize();
	if( err != paNoError )
	{
		printf("Initialization error\n");
		throw new std::exception();
	}

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
        printf("error setting up sampling");
        throw new std::exception();
    }

    _isInitialized = true;

    std::cout << "Sampling :: press ctrl+c to stop" << std::endl;


}

template <class T>
HSoundcardReader<T>::~HSoundcardReader()
{
    std::cout << "DESTRUCTED" << std::endl;

    if( _isInitialized )
    {
        // Make sure the stream is stopped
        Stop();

        // Terminate PortAudio
        PaError err = Pa_Terminate();
        if ( err != paNoError )
        {
            HError("Could not terminate PortAudio: " + std::string(Pa_GetErrorText(err)));
        }

        // Delete shared resources
        delete _cbd.buffer;
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
    if( _cbd.wrloc == _cbd.rdloc )
    {
        std::unique_lock<std::mutex> lck(_cbd.mtx);
        _cbd.cv.wait(lck);
    }

    if( _cbd.wrloc != _cbd.rdloc )
    {
        T* ptr = &_cbd.buffer[_cbd.rdloc];
        memcpy((void*) dest, (void*) ptr, _cbd.framesize * sizeof(T));

        static unsigned int blockLen = _cbd.framesize;
        static unsigned int lastPos = (NUMBER_OF_BUFFERS * _cbd.framesize) - 1;

        _cbd.rdloc += blockLen;
        if( _cbd.rdloc > lastPos )
        {
            _cbd.rdloc = 0;
        }

        return _cbd.framesize;
    }

    // No new data available, return zero write
    std::cout << "ZERO READ" << std::endl;
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
    static unsigned int blockLen = data->framesize;
    static unsigned int lastPos = (NUMBER_OF_BUFFERS * data->framesize) - 1;


    T* src = (T*) inputBuffer;
    T* dest = (T*) &data->buffer[data->wrloc];

    memcpy((void*) dest, (void*) src, framesPerBuffer * sizeof(T));


    data->wrloc += blockLen;
    if(data->wrloc > lastPos )
    {
        data->wrloc = 0;
    }


    std::unique_lock<std::mutex> lck(data->mtx);
    data->cv.notify_one();

    return 0;
}

template <class T>
bool HSoundcardReader<T>::Start()
{
    if( !_isInitialized )
    {
        HLog("Refusing to start a stream when not initialized");
        return false;
    }

    HLog("Starting soundcard stream");
    PaError err = Pa_StartStream( _stream );
    if( err != paNoError )
    {
        HError("Could not start stream: " + std::string(Pa_GetErrorText(err)));
        return false;
    }
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

    HLog("Stopping soundcard stream");
    PaError err = Pa_StopStream( _stream );
    if( err != paNoError )
    {
        HError("Could not stop stream: " + std::string(Pa_GetErrorText(err)));
        return false;
    }
    return true;
}

#endif
