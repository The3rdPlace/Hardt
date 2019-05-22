#ifndef __HWAVREADER_H
#define __HWAVREADER_H

template <class T>
class HWavReader : public HWav, public HFileReader<T>
{
    public:

        HWavReader(const char* filename);

        bool Start();
};

#endif