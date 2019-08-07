#ifndef __HWAVWRITER_H
#define __HWAVWRITER_H

/**
    Write samples to a wav file
*/
template <class T>
class HWavWriter : public HWav, public HFileWriter<T>
{
    private:

        size_t _size;

    public:

        /** Construct a new wav writer */
        HWavWriter(const char* filename, H_SAMPLE_FORMAT format, int channels, H_SAMPLE_RATE rate):
            HWav(filename, format, channels, rate),
            HFileWriter<T>(filename),
            _size(0)
        {}

        /** Construct a new wav writer */
        HWavWriter(const char* filename, H_SAMPLE_FORMAT format, int channels, H_SAMPLE_RATE rate, HWriterConsumer<T>* consumer):
            HWav(filename, format, channels, rate),
            HFileWriter<T>(filename, consumer),
            _size(0)
        {}

        /** Initialize before first write */
        bool Start();

        /** Complete the wav header with the final size, and cleanup after last write */
        bool Stop();

        /** Write to this wav stream */
        virtual int Write(T* dest, size_t blocksize);
};

#endif