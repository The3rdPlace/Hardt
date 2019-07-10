#ifndef __HLOWPASSBIQUAD_H
#define __HLOWPASSBIQUAD_H

template <class T>
class HLowpassBiQuad : public HBiQuad
{
    public:

        static HIirFilter<T>* Create(HWriter<T>* writer, size_t blocksize);
};

#endif