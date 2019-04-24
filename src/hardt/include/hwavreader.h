#ifndef __HWAVREADER_H
#define __HWAVREADER_H

template <class T>
class HWavReader : public HWav, HReader<T>
{
    public:

        HWavReader(HWav wav):
            HWav(wav)
        {
            throw new HNotImplementedException();
        }

        HWavReader(const char* filename):
            HWav(filename)
        {
            throw new HNotImplementedException();
        }

        int Read(T* dest, size_t blocksize)
        {
            throw new HNotImplementedException();
        }
};

#endif