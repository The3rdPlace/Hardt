#ifndef __HGAIN_H
#define __HGAIN_H

/**
    A filter providing gain (or attenuation)
*/
template <class T>
class HGain : public HFilter<T>
{
    private:

        float _gain;

    public:

        HGain(HWriter<T>* writer, float gain, size_t blocksize);
        HGain(HWriterConsumer<T>* consumer, float gain, size_t blocksize);
        HGain(HReader<T>* reader, float gain, size_t blocksize);

        ~HGain();

        virtual void Filter(T* src, T* dest, size_t blocksize);

        void SetGain(float gain);
};

#endif