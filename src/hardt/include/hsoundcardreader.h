#ifndef __HSOUNDCARDREADER_H
#define __HSOUNDCARDREADER_H

#define NUMBER_OF_BUFFERS 4

#include <stdio.h>
#include <iostream>



template <class T>
class HSoundcardReader : public HReader<T>
{
    private:

        // Todo, make more usefull object
        struct UD
        {
            int framesize;
            int wrloc;
            int rdloc;
            T* buffer;
            std::mutex mtx;
            std::condition_variable cv;
        };

        T* _buffer;
        UD _ud;

        PaStream *stream;

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
HSoundcardReader<T>::HSoundcardReader(int device, int rate, int channels, PaSampleFormat format, int framesPerBuffer)
{
    // Allocate buffers for data pushed from the card
    _buffer = new T[NUMBER_OF_BUFFERS * framesPerBuffer * sizeof(T)];

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

    _ud.framesize = framesPerBuffer;
    _ud.wrloc = 0;
    _ud.rdloc = 0;
    _ud.buffer = _buffer;

    // Get an input stream
    PaStreamFlags flags = paNoFlag;
    err = Pa_OpenStream(&stream, &inputParameters, NULL,
        rate,
        framesPerBuffer,   /* Frames pr. buffer */
        flags,
        callback,
        &_ud);

    if( err != paNoError )
    {
        printf("error setting up sampling");
        throw new std::exception();
    }

    std::cout << "Sampling :: press ctrl+c to stop" << std::endl;

    err = Pa_StartStream( stream );
    if( err != paNoError )
    {
        printf("error starting sampling");
        Pa_Terminate();
        throw new std::exception();
    }
}

template <class T>
HSoundcardReader<T>::~HSoundcardReader()
{
    std::cout << "DESTRUCTED" << std::endl;
    PaError err = Pa_StopStream( stream );
    if( err != paNoError )
    {
        printf("error stopping sampling");
        Pa_Terminate();
    }

	err = Pa_Terminate();
	if ( err != paNoError )
	{
		printf("Termination error\n");
	}

    delete _buffer;
}

template <class T>
int HSoundcardReader<T>::Blocksize()
{
    return _ud.framesize;
}

template <class T>
int HSoundcardReader<T>::Read(T* dest)
{
    if( _ud.wrloc == _ud.rdloc )
    {
        std::unique_lock<std::mutex> lck(_ud.mtx);
        _ud.cv.wait(lck);
    }

    if( _ud.wrloc != _ud.rdloc )
    {
        T* ptr = &_buffer[_ud.rdloc];
        memcpy((void*) dest, (void*) ptr, _ud.framesize * sizeof(T));

        static unsigned int blockLen = _ud.framesize;
        static unsigned int lastPos = (NUMBER_OF_BUFFERS * _ud.framesize) - 1;

        _ud.rdloc += blockLen;
        if( _ud.rdloc > lastPos )
        {
            _ud.rdloc = 0;
        }

        return _ud.framesize;
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
    UD *data = (UD*) userData;
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

#endif