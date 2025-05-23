#ifndef __HSOUNDCARDWRITER_H
#define __HSOUNDCARDWRITER_H

#include <mutex>
#include <condition_variable>

#include "hardt.h"

#define NUMBER_OF_BUFFERS 4

/**
    Send samples to a soundcard for audible output
*/
template <class T>
class HSoundcardWriter : public HWriter<T>
{
    private:

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

        PaStream *_stream;
        bool _isStarted;

        bool _isInitialized;

        bool Start();
        bool Stop();

        void Init(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer);

    public:

        /** Construct a new HSoundcardWriter */
        HSoundcardWriter(std::string id, int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer);

        /** Construct a new HSoundcardWriter and let it register with the upstream writer using the HWriterConsumer scheme */
        HSoundcardWriter(std::string id, int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer, HWriterConsumer<T>* consumer);

        /** Default destructor */
        ~HSoundcardWriter();

        /** Write a block of samples to the soundcard */
        int WriteImpl(T* src, size_t blocksize);

        /** Callback method, should only be called by the PortAudio layer */
        static int callback( const void *inputBuffer, void *outputBuffer,
                                   unsigned long framesPerBuffer,
                                   const PaStreamCallbackTimeInfo* timeInfo,
                                   PaStreamCallbackFlags statusFlags,
                                   void *userData );

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            // No further propagation of commands
            return true;
        }
};

#endif
