#ifndef __HWAVWRITER_H
#define __HWAVWRITER_H

template <class T>
class HWavWriter : public HWav, public HFileWriter<T>
{
    public:

        HWavWriter(const char* filename, H_SAMPLE_FORMAT format, int channels, H_SAMPLE_RATE rate, size_t size):
            HWav(filename, format, channels, rate, size),
            HFileWriter<T>(filename)
        {
            // Write wav-file header.
            // We know that the header is _always_ 44 bytes long for any wav
            // file, regardless of format, rate and number of channels. So we
            // can safely divide 44 by the size of the datatype (1, 2, 4)
            HFileWriter<T>::Write((T*) &_header, 44 / sizeof(T));
        }
};

#endif