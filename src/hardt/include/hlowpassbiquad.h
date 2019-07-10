#ifndef __HLOWPASSBIQUAD_H
#define __HLOWPASSBIQUAD_H

template <class T>
class HLowpassBiQuad : public HBiQuad
{
    private:

        static float* Calculate();

    public:

        static HIirFilter<T>* Create(HWriter<T>* writer, float fCutOff, float rate, float quality, float gain,  size_t blocksize);
        static HIirFilter<T>* Create(HReader<T>* reader, float fCutOff, float rate, float quality, float gain,  size_t blocksize);
};

#endif