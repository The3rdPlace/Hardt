#ifndef __HSOUNDCARDREADER_H
#define __HSOUNDCARDREADER_H

#include "hprobe.h"

#include <mutex>
#include <condition_variable>

#include "hardt.h"

#define NUMBER_OF_BUFFERS 4
#define DEFAULT_BLOCKSIZE 1024

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

        // A probe
        HProbe<T>* _probe;

        /**
         * Implements Start() to begin sampling
         */
        bool Start();

        /**
         * Implements Stop() to stop sampling
         */
        bool Stop();

    public:

        /**
         * Construct a new HSoundcardReader
         *
         * @param device Device id
         * @param rate Samplerate
         * @param channels Number of channels. If more than 1, then samples is returned multiplexed
         * @param format Sample format
         * @param blocksize Number of samples per chunk read
         * @param probe Probe
         */
        HSoundcardReader(std::string id, int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int blocksize = DEFAULT_BLOCKSIZE, HProbe<T>* probe = nullptr);

        /**
         * Default destructor
         */
        ~HSoundcardReader();

        /**
         * Read a block of samples from the soundcard
         *
         * @param dest Destination buffer
         * @param blocksize Number of samples to read
         */
        int Read(T* dest, size_t blocksize);

        /**
         * Callback method, should only be called by the PortAudio layer
         *
         * @param inputBuffer Pointer to buffer containing incomming samples
         * @param outputBuffer Pointer to buffer where output samples should be placed (not relevant for reading)
         * @param blocksize Number of samples to read before calling this callback
         * @param timeInfo Timing information from PortAudio API layer
         * @param statusFlags Callback flags for the PortAudio API layer
         * @param userData Pointer to a struct with user data (buffers, etc.)
         * */
        static int callback( const void *inputBuffer, void *outputBuffer,
                                   unsigned long blocksize,
                                   const PaStreamCallbackTimeInfo* timeInfo,
                                   PaStreamCallbackFlags statusFlags,
                                   void *userData );

        /**
         * Execute and/or pass on a command
         *
         * @param command Command
         */
        bool Command(HCommand* command) {
            // No further propagation of commands
            return true;
        }
};

#endif
