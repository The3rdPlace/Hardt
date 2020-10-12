#ifndef __HMULTIPLIER_H
#define __HMULTIPLIER_H

#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hprobe.h"

/**
    Multiplier (unlinear mixer).

    Will mix the input signal with a localoscillator signal thus
    producing  (Tin + Tlo) and (Tin - Tlo)

    The localoscillator frequency can be changed by calling
    SetFrequency(). behaviour while the frequency is transitioning
    is undefined, if you make large adjustments you may want to
    reduce the input level while setting the new frequency to avoid
    clicks or noise (check if it is a problem in your actual case)
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

        HProbe<T>* _probe;

    public:

        /** Construct a new HMultiplier (frequency mixer) */
        HMultiplier(HReader<T>* reader, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HMultiplier (frequency mixer) */
        HMultiplier(HWriter<T>* writer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HMultiplier (frequency mixer) */
        HMultiplier(HWriterConsumer<T>* consumer, H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize, HProbe<T>* probe = NULL);

        /** Default destructor */
        ~HMultiplier();

        /** Common initialization tasks */
        void Init(H_SAMPLE_RATE rate, int frequency, int oscillatorAmplitude, size_t blocksize);

        /** Read a block of samples */
        int Read(T* dest, size_t blocksize);

        /** Write a block of samples */
        int Write(T* src, size_t blocksize);

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