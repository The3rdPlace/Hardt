#ifndef __HBIQUADFILTER_H
#define __HBIQUADFILTER_H

template <class BIQUAD, class T>
class HBiQuadFilter: public HIirFilter<T>
{
    public:

        HBiQuadFilter(HWriter<T>* writer, float fCutOff, float rate, float quality, float gain,  size_t blocksize):
            HIirFilter<T>(writer, (BIQUAD(fCutOff, rate, quality, gain)).Calculate(), 5, blocksize)
        {}

        HBiQuadFilter(HReader<T>* reader, float fCutOff, float rate, float quality, float gain,  size_t blocksize):
            HIirFilter<T>(reader, (BIQUAD(fCutOff, rate, quality, gain)).Calculate(), 5, blocksize)
        {}

        ~HBiQuadFilter()
        {}

        virtual void Filter(T* src, T* dest, size_t blocksize)
        {
            HIirFilter<T>::Filter(src, dest, blocksize);
        }

        void SetCoefficients(float fCutOff, float rate, float quality, float gain,  size_t blocksize)
        {
            this->SetCoefficients((BIQUAD(fCutOff, rate, quality, gain)).Calculate(), 5);
        }
};

#endif