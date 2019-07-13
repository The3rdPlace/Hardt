#ifndef __HGAIN_H
#define __HGAIN_H

template <class T>
class HGain : public HFilter<T> //public HWriter<T>, public HReader<T>
{
    private:

        float _gain;

    public:

        HGain(HWriter<T>* writer, float gain, size_t blocksize);
        HGain(HReader<T>* reader, float gain, size_t blocksize);

        ~HGain();

        void Filter(T* src, T* dest, size_t blocksize);

        void SetGain(float gain);
};

#endif