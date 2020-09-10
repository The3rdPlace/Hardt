#ifndef __HRTL2832READER_CPP
#define __HRTL2832READER_CPP

#include <mutex>
#include <cstring>

#include "hrtl2832reader.h"

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HRtl2832Reader<T>::HRtl2832Reader(int device, H_SAMPLE_RATE rate, HRtl2832::MODE mode, int gain, int32_t frequency, int blocksize, bool directSampling, bool offset, int correction):
        _isInitialized(false),
        _isStarted(false),
        _mode(mode),
        _blocksize(blocksize)
{
    HLog("HRtl2832Reader(device=%d, rate=%d, framesPerBuffer=%d)", device, rate, blocksize);

    // Buffer length must be a multiple of 512 and maximum 16384/8192
    if( blocksize % 512 > 0 ) {
        HError("Blocksize must be a multiple of 512");
        throw new HInitializationException("Blocksize must be a multiple of 512");
    }
    if( blocksize > 16384 && mode == HRtl2832::MODE::IQ ) {
        HError("Blocksize must be less than or equal to 16384");
        throw new HInitializationException("Blocksize must be less than or equal to 16384");
    }
    if( blocksize > 8192 && (mode == HRtl2832::MODE::I || mode == HRtl2832::MODE::Q) ) {
        HError("Blocksize must be less than or equal to 8192");
        throw new HInitializationException("Blocksize must be less than or equal to 8192");
    }

    // Determine how many samples we need from the device per read.
    // Unless running in IQ mode (multiplexed I and Q values), we
    // need 2 times the number of samples from the device in order to
    // return 'blocksize' samples
    _buflen = _blocksize * (_mode == HRtl2832::MODE::IQ ? 1 : 2);

    // Allocate a temporary buffer for internal use
    _buffer = new T[_buflen];

    // If we are asked to return real samples, then allocate a FFT object
    // to handle the IQ-2-Real conversion
    if( mode == HRtl2832::MODE::REAL ) {
        _fft = new HFft<T>(_blocksize);
    }

    // Initialize resources used by the callback function
    _cbd.wrloc = 0;
    _cbd.rdloc = 0;
    _cbd.buffer = new unsigned char[NUMBER_OF_RTL_BUFFERS * _buflen];
    if( _cbd.buffer == NULL )
    {
        HError("Unable to allocate %d buffers for %d frames á %d bytes", NUMBER_OF_RTL_BUFFERS, _buflen, sizeof(unsigned char));
        throw new HInitializationException("Out of memory when allocating buffers");
    }
    HLog("%d buffers allocated for the device output", NUMBER_OF_RTL_BUFFERS * _buflen);

    // Error code from rtlsdr library api function calls
    int result;

    // Open the RTL-2832 device
    rtlsdr_open(&_dev, device);
    if( _dev == NULL ) {
        HError("Failed to open RTL-2832 device with index %d", device);
        throw new HInitializationException("Failed to open device");
    }
    HLog("RTL-2832 device with index %d opened", device);

    // Set direct sampling mode if requested
    if( directSampling ) {
        result = rtlsdr_set_direct_sampling(_dev, 1);
        if (result < 0) {
            HError("Failed to set RTL-2832 direct sampling mode");
            throw new HInitializationException("Failed to set direct sampling mode");
        }
        HLog("RTL-2832 direct sampling mode enabled");
    }

    // Set tuner offset, if needed
    if( offset ) {
        result = rtlsdr_set_offset_tuning(_dev, 1);
        if( result < 0 ) {
            HError("Failed to enable RTL-2832 tuner offset", offset);
            throw new HInitializationException("Failed to enable tuner offset");
        }
        HLog("RTL-2832 tuner offset enabled");
    } else {
        HLog("RTL-2832 tuner offset not required");
    }

    // Set frequency correction, if needed
    if( correction != 0 ) {
        result = rtlsdr_set_freq_correction(_dev, correction);
        if( result < 0 ) {
            HError("Failed to set RTL-2832 frequency correction to %d", correction);
            throw new HInitializationException("Failed to set frequency correction");
        }
        HLog("RTL-2832 frequency correction set to %d", rtlsdr_get_offset_tuning(_dev));
    } else {
        HLog("RTL-2832 frequency correction not required");
    }

    // Set sample rate
    result = rtlsdr_set_sample_rate(_dev, rate);
    if( result < 0 ) {
        HError("Failed to set RTL-2832 device samplerate to %d", rate);
        throw new HInitializationException("Failed to set samplerate");
    }
    HLog("RTL-2832 samplerate set to %d", rtlsdr_get_sample_rate(_dev));

    // Set center frequency (when not in direct sampling mode, otherwise it is
    // the of frequency for the Q-branch multiplier)
    if( !SetCenterFrequency(frequency) ) {
        throw new HInitializationException("Failed to set center frequency");
    }

    // Set gain
    if( !SetGain(gain) ) {
        throw new HInitializationException("Failed to set gain");
    }

    // Reset buffers (required)
    result = rtlsdr_reset_buffer(_dev);
    if( result < 0 ) {
        HError("Failed to reset RTL-2832 buffers");
        throw new HInitializationException("Failed to reset buffers");
    }
    HLog("RTL-2832 buffers reset");

    // Ready
    _isInitialized = true;
}

template <class T>
HRtl2832Reader<T>::~HRtl2832Reader()
{
    HLog("~HRtl2832Reader()");

    if( _isInitialized )
    {
        // Make sure the stream is stopped
        Stop();
        HLog("Stream is stopped");

        // Close the device
        rtlsdr_close(_dev);

        // Delete shared resources
        delete _cbd.buffer;
        HLog("Buffers released");
    }

    // If we have an FFT object, delete it
    if( _mode == HRtl2832::MODE::REAL ) {
        delete _fft;
    }

    // Delete temporary buffer
    delete _buffer;
}

template <class T>
int HRtl2832Reader<T>::Read(T* dest, size_t blocksize)
{
    // Always read the configured blocksize
    if( blocksize != _blocksize )
    {
        Stop();
        throw new HAudioIOException("It is not allowed to read  more or less data than what was configured");
    }

    // Make sure we are running
    if( !_isStarted )
    {
        throw new HAudioIOException("Stream is not started, no data to read");
    }

    // If read and write position is the same (same buffer), then wait for new samples
    if( _cbd.wrloc == _cbd.rdloc )
    {
        std::unique_lock<std::mutex> lck(_cbd.mtx);
        _cbd.lock.wait(lck);
    }

    // If we have samples available, then read the next buffer
    if( _cbd.wrloc != _cbd.rdloc )
    {
        unsigned char* src = &_cbd.buffer[_cbd.rdloc];
        switch( _mode ) {

            // Multiplexed I and Q values
            case HRtl2832::MODE::IQ: {
                for( int i = 0; i < blocksize; i++ ) {
                    dest[i] = ((T) src[i]) - 127;
                }
                break;
            }

            // Return I channel
            case HRtl2832::MODE::I: {
                for( int i = 0; i < _buflen; i += 2 ) {
                    dest[i / 2] = ((T) src[i]) - 127;
                }
                break;
            }

            // Return Q channel
            case HRtl2832::MODE::Q: {
                for( int i = 1; i < _buflen; i += 2 ) {
                    dest[i / 2] = ((T) src[i]) - 127;
                }
                break;
            }

            // Return realvalued time-domain signal
            case HRtl2832::MODE::REAL: {
                for( int i = 0; i < _buflen; i++ ) {
                    _buffer[i] = ((T) src[i]) - 127;
                }
                _fft->IQ2REAL(_buffer, dest);
                break;
            }
        }

        // Advance read position to next buffer, if we have read the last buffer,
        // then wrap around to the first buffer.
        _cbd.rdloc += blocksize;
        if( _cbd.rdloc >= (NUMBER_OF_RTL_BUFFERS * _buflen) )
        {
            _cbd.rdloc = 0;
        }

        // We always reads the entire buffer as given
        return blocksize;
    }

    // No new data available, return zero write.
    // Most likely, this indicates that we are shutting down (unclean termination)
    HLog("No data available for read()");
    return 0;
}

template <class T>
void HRtl2832Reader<T>::callback(unsigned char* buffer, uint32_t length, void* ctx) {

    // Cast data passed through stream to our structure.
    CallbackData *data = (CallbackData*) ctx;

    // Copy new data from the soundcard to the buffer
    // Buffer is always unsigned char, so no need to use 'sizeof(unsigned char)'
    memcpy((void*) &data->buffer[data->wrloc], (void*) buffer, length);

    // Advance write position to next buffer, if we have written the last buffer,
    // then wrap around to the first buffer.
    data->wrloc += length;
    if( data->wrloc >= (NUMBER_OF_RTL_BUFFERS * length) )
    {
        data->wrloc = 0;
    }

    // We might have a waiting reader, signal that data is available
    // Since we are not waiting on the mutex, using it here should be fine.
    std::unique_lock<std::mutex> lck(data->mtx);
    data->lock.notify_one();
}

template <class T>
bool HRtl2832Reader<T>::Start()
{
    if( !_isInitialized )
    {
        HLog("Refusing to start a stream when not initialized");
        return false;
    }

    if( !_isStarted ) {

        HLog("Starting input stream");
        _current = new std::thread([this]() {

            // Start async reader thread
            HLog("Starting async reader thread with buflen=%d", _buflen);
            if( rtlsdr_read_async(_dev, callback, (void *) &_cbd, NUMBER_OF_RTL_BUFFERS, _buflen) < 0 ) {
                HError("Failed to start the rtl device");
                _isStarted = false;
            }

            // Reset when the read thread returns
            _current = NULL;
            _isStarted = false;
            HLog("Async reader thread stopped");
        });
    } else {
        HLog("Already started");
    }

    HLog("Input stream started");
    _isStarted = true;
    return true;
}

template <class T>
bool HRtl2832Reader<T>::Stop()
{
    if( !_isInitialized ) {

        HLog("Refusing to stop a stream when not initialized");
        return false;
    }

    if( _isStarted) {

        HLog("Stopping input stream");
        rtlsdr_cancel_async(_dev);
        _current->join();
        _isStarted = false;
        HLog("Joined reader thread");
    } else {
        HLog("Stream is already stopped, all is good");
    }

    return true;
}

template <class T>
bool HRtl2832Reader<T>::EnableBiasTee(int gpio) {

    if( gpio > 0 ) {
        if( rtlsdr_set_bias_tee_gpio(_dev, gpio, 1) == 0 ) {
            HLog("Enabled bias-tee output voltage on gpio %d", gpio);
            return true;
        }
        HError("Unable to enable bias-tee output voltage on gpio %d", gpio);
        return false;
    } else {
        if (rtlsdr_set_bias_tee(_dev, 1) == 0) {
            HLog("Enabled bias-tee output voltage");
            return true;
        }
        HError("Unable to enable bias-tee output voltage");
        return false;
    }
}

template <class T>
bool HRtl2832Reader<T>::DisableBiasTee(int gpio) {

    if( gpio > 0 ) {
        if( rtlsdr_set_bias_tee_gpio(_dev, gpio, 0) == 0 ) {
            HLog("Disabled bias-tee output voltage on gpio %d", gpio);
            return true;
        }
        HError("Unable to disable bias-tee output voltage on gpio %d", gpio);
        return false;
    } else {
        if (rtlsdr_set_bias_tee(_dev, 0) == 0) {
            HLog("Disabled bias-tee output voltage");
            return true;
        }
        HError("Unable to disable bias-tee output voltage");
        return false;
    }
}

template <class T>
bool HRtl2832Reader<T>::SetCenterFrequency(uint32_t frequency) {
    if (rtlsdr_set_center_freq(_dev, frequency) < 0) {
        HError("Failed to set RTL-2832 device center frequency to %d", frequency);
        return false;
    }
    HLog("RTL-2832 center frequency set to %d", rtlsdr_get_center_freq(_dev));
    return true;
}

template <class T>
uint32_t HRtl2832Reader<T>::GetCenterFrequency() {
    return rtlsdr_get_center_freq(_dev);
}

template <class T>
bool HRtl2832Reader<T>::SetGain(int gain) {
    if( 0 == gain ) {
        if( rtlsdr_set_tuner_gain_mode(_dev, 0) < 0 ) {
            HError("Failed to set RTL-2832 device to automatic gain");
            return false;
        }
        HLog("RTL-2832 set to use automatic gain");
    } else {
        if( rtlsdr_set_tuner_gain_mode(_dev, 1) < 0 ) {
            HError("Failed to set RTL-2832 to use manual gain");
            return false;
        }
        HLog("RTL-2832 manual gain mode enabled");

        if( rtlsdr_set_tuner_gain(_dev, gain) < 0 ) {
            HError("Failed to set RTL-2832 manual gain level %d", gain);
            return false;
        }
        HLog("RTL-2832 gain set to %d (%f dB)", rtlsdr_get_tuner_gain(_dev), rtlsdr_get_tuner_gain(_dev) / 10.0);
    }
    return true;
}

template <class T>
int HRtl2832Reader<T>::GetGain() {
    return rtlsdr_get_tuner_gain(_dev);
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HRtl2832Reader()
template
HRtl2832Reader<int8_t>::HRtl2832Reader(int device, H_SAMPLE_RATE rate, HRtl2832::MODE mode, int gain, int32_t frequency, int blocksize, bool directSampling, bool offset, int correction);

template
HRtl2832Reader<uint8_t>::HRtl2832Reader(int device, H_SAMPLE_RATE rate, HRtl2832::MODE mode, int gain, int32_t frequency, int blocksize, bool directSampling, bool offset, int correction);

template
HRtl2832Reader<int16_t>::HRtl2832Reader(int device, H_SAMPLE_RATE rate, HRtl2832::MODE mode, int gain, int32_t frequency, int blocksize, bool directSampling, bool offset, int correction);

template
HRtl2832Reader<int32_t>::HRtl2832Reader(int device, H_SAMPLE_RATE rate, HRtl2832::MODE mode, int gain, int32_t frequency, int blocksize, bool directSampling, bool offset, int correction);

// ~HRtl2832Reader()
template
HRtl2832Reader<int8_t>::~HRtl2832Reader();

template
HRtl2832Reader<uint8_t>::~HRtl2832Reader();

template
HRtl2832Reader<int16_t>::~HRtl2832Reader();

template
HRtl2832Reader<int32_t>::~HRtl2832Reader();

// Read()
template
int HRtl2832Reader<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HRtl2832Reader<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HRtl2832Reader<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HRtl2832Reader<int32_t>::Read(int32_t* dest, size_t blocksize);

// SetCenterFrequency
template
bool HRtl2832Reader<int8_t>::SetCenterFrequency(uint32_t frequency);

template
bool HRtl2832Reader<uint8_t>::SetCenterFrequency(uint32_t frequency);

template
bool HRtl2832Reader<int16_t>::SetCenterFrequency(uint32_t frequency);

template
bool HRtl2832Reader<int32_t>::SetCenterFrequency(uint32_t frequency);

// GetCenterFrequency
template
uint32_t HRtl2832Reader<int8_t>::GetCenterFrequency();

template
uint32_t HRtl2832Reader<uint8_t>::GetCenterFrequency();

template
uint32_t HRtl2832Reader<int16_t>::GetCenterFrequency();

template
uint32_t HRtl2832Reader<int32_t>::GetCenterFrequency();

// SetGain
template
bool HRtl2832Reader<int8_t>::SetGain(int gain);

template
bool HRtl2832Reader<uint8_t>::SetGain(int gain);

template
bool HRtl2832Reader<int16_t>::SetGain(int gain);

template
bool HRtl2832Reader<int32_t>::SetGain(int gain);

// GetGain
template
int HRtl2832Reader<int8_t>::GetGain();

template
int HRtl2832Reader<uint8_t>::GetGain();

template
int HRtl2832Reader<int16_t>::GetGain();

template
int HRtl2832Reader<int32_t>::GetGain();

//! @endcond
#endif
