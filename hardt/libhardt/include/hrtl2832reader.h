#ifndef __HRTL2832READER_H
#define __HRTL2832READER_H

#include <mutex>
#include <condition_variable>
#include "rtl-sdr.h"

#include "hardt.h"
#include "hreader.h"

#define NUMBER_OF_BUFFERS 4
#define DEFAULT_FRAMESIZE 1024

/**
    Read samples from an RTL-2832 device
*/
template <class T>
class HRtl2832Reader : public HReader<T>
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
    //PaStream *_stream;
    bool _isStarted;

    // Housekeeping
    bool _isInitialized;

    // Implement Start() and Stop() so that we can start sampling whe
    // data is needed, and stop again when the reader is no longer active
    bool Start();
    bool Stop();

public:

    /** Construct a new HRtl2832Reader */
    HRtl2832Reader(int device, H_SAMPLE_RATE rate, int channels, H_SAMPLE_FORMAT format, int framesPerBuffer = DEFAULT_FRAMESIZE);

    /** Default destructor */
    ~HRtl2832Reader();

    /** Read a block of samples from the soundcard */
    int Read(T* dest, size_t blocksize);

    /** Callback method, should only be called by the PortAudio layer */
    /*static int callback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData );*/

    /** Execute and/or pass on a command */
    bool Command(HCommand* command) {
        // No further propagation of commands
        return true;
    }
};

#endif
