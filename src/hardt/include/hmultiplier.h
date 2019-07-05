#ifndef __HMultiplier_H
#define __HMultiplier_H

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

        HMultiplier(HReader<T>* reader, H_SAMPLE_RATE rate, int frequency, size_t blocksize);

        ~HMultiplier();

        int Read(T* dest, size_t blocksize);

        bool Start();
        bool Stop();
};

#endif