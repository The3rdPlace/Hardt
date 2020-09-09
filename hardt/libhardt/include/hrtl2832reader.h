#ifndef __HRTL2832READER_H
#define __HRTL2832READER_H

#include <mutex>
#include <condition_variable>
#include <thread>
#include "rtl-sdr.h"

#include "hardt.h"
#include "hobject.h"
#include "hreader.h"
#include "hfft.h"
#include "hrtl2832.h"

#define NUMBER_OF_RTL_BUFFERS 15

/**
    Read samples from an RTL-2832 device
*/
template <class T>
class HRtl2832Reader : public HReader<T>
{
    public:

    private:

        HRtl2832::MODE _mode;
        int _buflen;
        HFft<T>* _fft;
        T* _buffer;
        std::thread* _current;
        rtlsdr_dev_t *dev;
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

        // Indicate if the device has been started
        bool _isStarted;

        // Indicate if the device has been initialize
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
              mode: The reader mode (IQ, I, Q, REAL)
              gain: The device gain (0 to use automatic)
              frequency: Initial center- or if frequency
              blocksize: Number of samples to read from the device in a single read operation
                         If mode is set to 'IQ', then the returned amount of samples will be
                         'blocksize/2' since you will get multiplexed IQ samples. The buffer
                         must however still be of length 'blocksize' due to the multiplexing
              directSampling: Set to true to use direct sampling. Most RTL-2832 dongles will then read the
                              Q-channel, so use mode=Q
              offset: Set to true to use tuner offset mode (only E4000 tuners)
              correct: Frequency correction in ppm
         * */
        HRtl2832Reader(int device, H_SAMPLE_RATE rate, HRtl2832::MODE mode, int gain, int32_t frequency, int blocksize, bool directSampling = false, bool offset = 0, int correction = 0);

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

        /** Enable the 5 volt bias-tee output voltage on the antenna connector

            Arguments:
              gpio = gpio pin. Use 0 (zero) to use default gpio pin
        */
        bool EnableBiasTee(int gpio = 0);

        /** Disable the 5 volt bias-tee output voltage on the antenna connector

            Arguments:
              gpio = gpio pin. Use 0 (zero) to use default gpio pin
        */
        bool DisableBiasTee(int gpio = 0);
};

#endif
