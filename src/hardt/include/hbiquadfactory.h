#ifndef __HBIQUADFACTORY_H
#define __HBIQUADFACTORY_H

template <class BIQUAD, class T>
class HBiQuadFactory
{
    public:

        static HIirFilter<T>* Create(HWriter<T>* writer, float fCutOff, float rate, float quality, float gain,  size_t blocksize)
        {
            BIQUAD* bq = new BIQUAD(fCutOff, rate, quality, gain);
            return new HIirFilter<T>(writer, ((HBiQuad<T>*) bq)->Calculate(), 5, blocksize);
        }

        static HIirFilter<T>* Create(HReader<T>* reader, float fCutOff, float rate, float quality, float gain,  size_t blocksize)
        {
            BIQUAD* bq = new BIQUAD(fCutOff, rate, quality, gain);
            return new HIirFilter<T>(reader, ((HBiQuad<T>*) bq)->Calculate(), 5, blocksize);
        }
};

#endif