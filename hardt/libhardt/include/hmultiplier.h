#ifndef __HMULTIPLIER_H
#define __HMULTIPLIER_H

#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"

/**
    Multiplier (unlinear mixer).

    Will mix the input signal with a localoscillator signal thus
    producing  (Tin + Tlo) and (Tin - Tlo)

    The localoscillator frequency can be changed by calling
    SetFrequency(). behaviour while the frequency is transitioning
    is undefined, if you make large adjustments you may want to
    reduce the input level while setting the new frequency to avoid
    clicks or noise (check if it is a problem in your actual case)

    The multiplier can be created with any amplitude of the local
    oscillator, but the output level is scaled down, so that the
    gain factor of the multiplier is always 1. So select a local
    oscillator amplitude that matches the input signal, and prevents
    internal overruns (AMPL_in * AMPL_lo <= max(T)/2)
*/
template <class T>
class HMultiplier : public HReader<T>, public HWriter<T>, public HWriterConsumer<T>
{
    private:

        HReader<T>* _reader;
        HWriter<T>* _writer;
        T* _buffer;

        int _blocksize;

        HLocalOscillator<T>* _localOscillator;
        int _scaling;

    public:

        /** Construct a new HMultiplier (frequency mixer) */
        HMultiplier(std::string id, HReader<T>* reader, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize);

        /** Construct a new HMultiplier (frequency mixer) */
        HMultiplier(std::string id, HWriter<T>* writer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize);

        /** Construct a new HMultiplier (frequency mixer) */
        HMultiplier(std::string id, HWriterConsumer<T>* consumer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize);

        /** Default destructor */
        ~HMultiplier();

        /** Common initialization tasks */
        void Init(std::string id, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize);

        /** Read a block of samples */
        int ReadImpl(T* dest, size_t blocksize);

        /** Write a block of samples */
        int WriteImpl(T* src, size_t blocksize);

        /** Set writer */
        void SetWriter(HWriter<T>* writer)
        {
            _writer = writer;
        }

        /** Initialize before first read */
        bool Start();

        /** Cleanup after last read */
        bool Stop();

        /** Set the frequency of the local oscillator that feeds one of the inputs of the mixer */
        void SetFrequency(int frequency);

        /** Mix two signals */
        void Mix(T* src, T* dest, size_t blocksize);

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            if( _reader != nullptr ) {
                if( !_reader->Command(command) )
                {
                    return false;
                }
            }
            if( _writer != nullptr ) {
                if( !_writer->Command(command) )
                {
                    return false;
                }
            }
            return true;
        }
};

#endif