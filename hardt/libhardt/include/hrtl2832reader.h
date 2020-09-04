#ifndef __HRTL2832READER_H
#define __HRTL2832READER_H

#include <mutex>
#include <condition_variable>
#include "rtl-sdr.h"

#include "hardt.h"
#include "hreader.h"

#define NUMBER_OF_RTL_BUFFERS 15

/**
    Read samples from an RTL-2832 device
*/
template <class T>
class HRtl2832Reader : public HReader<T>
{
    public:

        /** The reader mode:

           IQ: Return multiplexed IQ samples
            I: Return only I samples
            Q: Return only Q samples
            REAL: Return realvalued signal (warning: this might cost cycles, use with care and low samplerate)
        */
        enum MODE {IQ, I, Q, REAL};

    private:

        MODE _mode;

        int _buflen;

        HFft<T>* _fft;
        T* _buffer;

        std::thread* _current;
        rtlsdr_dev_t *dev;
        std::mutex mtx;
        std::condition_variable lock;
        size_t _blocksize;

        // Data accessed by the (static) callback function
        struct CallbackData
        {
            int wrloc;
            int rdloc;
            unsigned char* buffer;
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

        /** Construct a new HRtl2832Reader

            Arguments:
              device: The device id of the selected RTL-2832 device (usually 0-indexed)
              rate: The samplerate to set on the RTL-2832 device
              mode: The reader mode (IQ, I, Q)
              blocksize: Number of samples to read from the device in a single read operation
                         If mode is set to 'IQ', then the returned amount of samples will be
                         'blocksize/2' since you will get multiplexed IQ samples. The buffer
                         must however still be of length 'blocksize' due to the multiplexing

         * */
        HRtl2832Reader(int device, H_SAMPLE_RATE rate, MODE mode, int blocksize);

        /** Default destructor */
        ~HRtl2832Reader();

        /** Read a block of samples from the soundcard */
        int Read(T* dest, size_t blocksize);

        /** Callback method, should only be called by librtl */
        static void callback( unsigned char *buffer,
                             uint32_t length,
                             void* context);

        /** Execute and/or pass on a command */
        bool Command(HCommand* command) {
            // No further propagation of commands
            return true;
        }
};

#endif
