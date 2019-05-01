#ifndef __HWAVWRITER_H
#define __HWAVWRITER_H

template <class T>
class HWavWriter : public HWav, public HFileWriter<T>
{
    private:

        size_t _size;

    public:

        HWavWriter(const char* filename, H_SAMPLE_FORMAT format, int channels, H_SAMPLE_RATE rate):
            HWav(filename, format, channels, rate),
            HFileWriter<T>(filename),
            _size(0)
        {}

        bool Start(void* unused);

        bool Stop();

        virtual int Write(T* dest, size_t blocksize);
};

#endif