#ifndef __HMultiplier_H
#define __HMultiplier_H

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
class HMultiplier : public HReader<T>
{
    private:

        HReader<T>* _reader;
        T* _buffer;

        int _blocksize;

        HLocalOscillator<float>* _localOscillator;
        float* _oscillatorBuffer;

    public:

        /** Construct a new HMultiplier (frequency mixer) */
        HMultiplier(HReader<T>* reader, H_SAMPLE_RATE rate, int frequency, size_t blocksize);

        /** Default destructor */
        ~HMultiplier();

        /** Read a block of samples */
        int Read(T* dest, size_t blocksize);

        /** Initialize before first read */
        bool Start();

        /** Cleanup after last read */
        bool Stop();

        /** Set the frequency of the local oscillator that feeds one of the inputs of the mixer */
        void SetFrequency(int frequency);
};

#endif