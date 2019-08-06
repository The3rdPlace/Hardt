#ifndef __HSOUNDCARDREADER_H
#define __HSOUNDCARDREADER_H

#include <mutex>
#include <condition_variable>

#define NUMBER_OF_BUFFERS 4
#define DEFAULT_FRAMESIZE 1024

/**
    Read sound from a soundcard
*/
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
            std::mutex mtx;
            std::condition_variable lock;
        } _cbd;

        // The sample stream from the soundcard
        PaStream *_stream;
        bool _isStarted;

        // Housekeeping
        bool _isInitialized;

        // Implement Start() and Stop() so that we can start sampling whe
        // data is needed, and stop again when the reader is no longer active
        bool Start();
        bool Stop();

    public:

        HSoundcardReader(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer = DEFAULT_FRAMESIZE);
        ~HSoundcardReader();
        int Read(T* dest, size_t blocksize);

        static int callback( const void *inputBuffer, void *outputBuffer,
                                   unsigned long framesPerBuffer,
                                   const PaStreamCallbackTimeInfo* timeInfo,
                                   PaStreamCallbackFlags statusFlags,
                                   void *userData );
};

#endif
