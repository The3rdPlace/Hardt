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
#include "hprobe.h"
#include "hcommand.h"
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
        rtlsdr_dev_t *_dev;
        size_t _blocksize;
        HProbe<T>* _probe;

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

        // Indicate if the device has been initialized
        bool _isInitialized;

        /**
         * Implements Start()
         */
        bool Start();

        /**
         * Implements Stop()
         */
        bool Stop();

        void CopyBlock(T* dest, unsigned char* src, size_t blocksize);

    public:

        /**
         * Construct a new HRtl2832Reader
         *
         * @param id Element identifier
         * @param device The device id of the selected RTL-2832 device (usually 0-indexed)
         * @param rate The samplerate to set on the RTL-2832 device
         * @param mode The reader mode (IQ, I, Q, REAL). I or Q enables direct sampling.
         * @param gain The device gain (0 to use automatic)
         * @param frequency Initial center- or if frequency
         * @param blocksize Number of samples to read from the device in a single read operation
         *                  If mode is set to 'IQ', then the returned amount of samples will be
         *                  'blocksize/2' since you will get multiplexed IQ samples. The buffer
         *                  must however still be of length 'blocksize' due to the multiplexing
         * @param offset Set to true to use tuner offset mode (only E4000 tuners)
         * @param correction Frequency correction in ppm
         * @param probe Probe
         */
        HRtl2832Reader(std::string id, int device, H_SAMPLE_RATE rate, HRtl2832::MODE mode, int gain, int32_t frequency, int blocksize, bool offset = 0, int correction = 0, HProbe<T>* probe = nullptr);

        /**
         * Default destructor
         */
        ~HRtl2832Reader();

        /**
         * Read a block of samples from the soundcard
         *
         * @param dest Destination buffer
         * @param blocksize Number of samples to read
         */
        int ReadImpl(T* dest, size_t blocksize);

        /**
         * Callback method, should only be called by librtl
         *
         * @param buffer Buffer containing samples (actually a pointer to the area in the internal
         *               buffer where the newest samples has been stored.)
         * @param length Number of samples in the buffer
         * @param context Pointer to the context object provided at construction
         */
        static void callback( unsigned char *buffer, uint32_t length, void* context);

        /**
         * Execute and/or pass on a command
         */
        bool Command(HCommand* command) {

            // Handle known commands
            switch( command->Class ) {
                case H_COMMAND_CLASS::TUNER: {
                    switch( command->Opcode ) {
                        case H_COMMAND_OPCODE::SET_FREQUENCY: {
                            return SetCenterFrequency(command->Data.Value);
                        }
                        case H_COMMAND_OPCODE::SET_GAIN: {
                            return SetGain(command->Data.Value);
                        }
                    }
                }
                case H_COMMAND_CLASS::ANY: {
                    switch( command->Opcode ) {
                        case H_COMMAND_OPCODE::SET_GAIN: {
                            return SetGain(command->Data.Value);
                        }
                    }
                }
            }

            // Any other command returns true
            return true;
        }

        /**
         * Enable the 5 volt bias-tee output voltage on the antenna connector
         * Warning: Enabling bias tee voltage if you have a DC shorted antenna
         * connected will damage your rtl-sdr dongle (over time)
         *
         * @param gpio Gpio pin. Use 0 (zero) to use default gpio pin
         */
        bool EnableBiasTee(int gpio = 0);

        /**
         * Disable the 5 volt bias-tee output voltage on the antenna connector
         *
         * @param gpio Gpio pin. Use 0 (zero) to use default gpio pin
        */
        bool DisableBiasTee(int gpio = 0);

        /**
         * Set center frequency
         *
         * @param frequency The frequency to tune to
         * @return true if successfull, otherwise false
         */
        bool SetCenterFrequency(uint32_t frequency);

        /**
         * Get current centerfrequency
         *
         * @return Current center frequency (in Hz)
         */
        uint32_t GetCenterFrequency();

        /**
         * Set gain. Setting gain to 0 (zero) enables automatic gain
         *
         * @param gain Gain value
         * @return True if successfull, otherwise false
         */
        bool SetGain(int gain);

        /**
         * Get current gain. 0 (zero) means 'automatic'
         *
         * @return Gain value
         */
        int GetGain();
};

#endif
