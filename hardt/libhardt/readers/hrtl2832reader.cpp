#ifndef __HRTL2832READER_CPP
#define __HRTL2832READER_CPP

#include <stdio.h>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <cstring>

#include "hrtl2832reader.h"

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HRtl2832Reader<T>::HRtl2832Reader(int device, H_SAMPLE_RATE rate, MODE mode, int blocksize):
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
    if( blocksize > 16384 && mode == MODE::IQ ) {
        HError("Blocksize must be less than or equal to 16384");
        throw new HInitializationException("Blocksize must be less than or equal to 16384");
    }
    if( blocksize > 8192 && (mode == MODE::I || mode == MODE::Q) ) {
        HError("Blocksize must be less than or equal to 8192");
        throw new HInitializationException("Blocksize must be less than or equal to 8192");
    }

    // Determine how many samples we need from the device per read.
    // Unless running in IQ mode (multiplexed I and Q values), we
    // need 2 times the number of samples from the device in order to
    // return 'blocksize' samples
    _buflen = _blocksize * (_mode == MODE::IQ ? 1 : 2);

    // Allocate a temporary buffer for internal use
    _buffer = new T[_buflen];

    // If we are asked to return real samples, then allocate a FFT object
    // to handle the IQ-2-Real conversion
    if( mode == MODE::REAL ) {
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
    HLog("Buffers allocated");

    // Pass these as fields
    uint32_t dev_index = 0;
    int ppm_correction = 0;
    uint32_t freq = 126600000;//144570200;//122650000;//126900000; //144470200; //126800000; //50371000; //1440000; //50271000;//126798600;
    int gain = 0;
    int direct_sampling = 0;
    bool enable_biastee = false;
    int r;

    rtlsdr_open(&dev, dev_index);
    if (NULL == dev) {
        HError("Failed to open RTL-2832 device %d", dev_index);
        throw new HInitializationException("Failed to open device");
    }

    /* Set the tuner error */
    /*int r = rtlsdr_set_freq_correction(dev, ppm_correction);
    if (r < 0) {
        fprintf(stderr, "WARNING: Failed to set ppm error.\n");
        HError("stop %d", __LINE__);
    } else {
        fprintf(stderr, "Tuner error set to %i ppm.\n", ppm_correction);
        HError("stop %d", __LINE__);
    }*/

    /* Set the sample rate */
    r = rtlsdr_set_sample_rate(dev, 1152000); //(uint32_t) rate);
    if (r < 0) {
        fprintf(stderr, "WARNING: Failed to set sample rate.\n");
        HError("stop %d", __LINE__);
    }

    /* Set the frequency */
    r = rtlsdr_set_center_freq(dev, freq);
    if (r < 0) {
        HLog("WARNING: Failed to set center freq");
    }
    else {
        HLog("Tuned to %i Hz.", freq);
    }

    if (0 == gain) {
        /* Enable automatic gain */
        r = rtlsdr_set_tuner_gain_mode(dev, 0);
        if (r < 0) {
            fprintf(stderr, "WARNING: Failed to enable automatic gain.\n");
            HError("stop %d", __LINE__);
        }
    } else {
        HError("stop %d", __LINE__);
        /* Enable manual gain */
        r = rtlsdr_set_tuner_gain_mode(dev, 1);
        if (r < 0)
            fprintf(stderr, "WARNING: Failed to enable manual gain.\n");

        /* Set the tuner gain */
        r = rtlsdr_set_tuner_gain(dev, gain);
        if (r < 0)
            HLog("WARNING: Failed to set tuner gain.");
        else
            HLog("Tuner gain set to %f dB.", gain/10.0);
    }

    /*r = rtlsdr_set_bias_tee(dev, enable_biastee ? 1 : 0);
    if( r < 0 ) {
        HError("stop %d", __LINE__);
    }
    if (enable_biastee) {
        fprintf(stderr, "activated bias-T on GPIO PIN 0\n");
    }*/
    //r = rtlsdr_set_direct_sampling(dev, direct_sampling);
    /*r = rtlsdr_set_direct_sampling(dev, 0);
    if( r < 0 ) {
        HError("stop %d", __LINE__);
    }*/

    /* Reset endpoint before we start reading from it (mandatory) */
    r = rtlsdr_reset_buffer(dev);
    if (r < 0) {
        fprintf(stderr, "WARNING: Failed to reset buffers.\n");
        HError("stop %d", __LINE__);
    }

    HLog("center_freq = %d", rtlsdr_get_center_freq(dev));
    HLog("direct_sampling = %d", rtlsdr_get_direct_sampling(dev));
    HLog("freq_correction = %d", rtlsdr_get_freq_correction(dev));
    HLog("offset_tuning = %d", rtlsdr_get_offset_tuning(dev));
    HLog("sample_rate = %d", rtlsdr_get_sample_rate(dev));
    HLog("tuner_gain = %d", rtlsdr_get_tuner_gain(dev));

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
        rtlsdr_close(dev);

        // Delete shared resources
        delete _cbd.buffer;
        HLog("Buffers released");
    }

    // If we have an FFT object, delete it
    if( _mode == MODE::REAL ) {
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
            case MODE::IQ: {
                for( int i = 0; i < blocksize; i++ ) {
                    dest[i] = ((T) src[i]) - 127;
                }
                break;
            }

            // Return I channel
            case MODE::I: {
                for( int i = 0; i < _buflen; i += 2 ) {
                    dest[i / 2] = ((T) src[i]) - 127;
                }
                break;
            }

            // Return Q channel
            case MODE::Q: {
                for( int i = 1; i < _buflen; i += 2 ) {
                    dest[i / 2] = ((T) src[i]) - 127;
                }
                break;
            }

            // Return realvalued time-domain signal
            case MODE::REAL: {
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
            if( rtlsdr_read_async(dev, callback, (void *) &_cbd, NUMBER_OF_RTL_BUFFERS, _buflen) < 0 ) {
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
        rtlsdr_cancel_async(dev);
        _current->join();
        _isStarted = false;
        HLog("Joined reader thread");
    } else {
        HLog("Stream is already stopped, all is good");
    }

    return true;
}


/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HRtl2832Reader()
template
HRtl2832Reader<int8_t>::HRtl2832Reader(int device, H_SAMPLE_RATE rate, MODE mode, int blocksize);

template
HRtl2832Reader<uint8_t>::HRtl2832Reader(int device, H_SAMPLE_RATE rate, MODE mode, int blocksize);

template
HRtl2832Reader<int16_t>::HRtl2832Reader(int device, H_SAMPLE_RATE rate, MODE mode, int blocksize);

template
HRtl2832Reader<int32_t>::HRtl2832Reader(int device, H_SAMPLE_RATE rate, MODE mode, int blocksize);

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

//! @endcond
#endif
