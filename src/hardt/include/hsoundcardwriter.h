#ifndef __HSOUNDCARDWRITER_H
#define __HSOUNDCARDWRITER_H

#include <mutex>
#include <condition_variable>

#define NUMBER_OF_BUFFERS 4

/**
    Send samples to a soundcard for audible output
*/
template <class T>
class HSoundcardWriter : public HWriter<T>
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

        void Init(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer);

    public:

        HSoundcardWriter(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer);
        HSoundcardWriter(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer, HWriterConsumer<T>* consumer);
        ~HSoundcardWriter();
        int Write(T* src, size_t blocksize);

        static int callback( const void *inputBuffer, void *outputBuffer,
                                   unsigned long framesPerBuffer,
                                   const PaStreamCallbackTimeInfo* timeInfo,
                                   PaStreamCallbackFlags statusFlags,
                                   void *userData );
};

#endif
