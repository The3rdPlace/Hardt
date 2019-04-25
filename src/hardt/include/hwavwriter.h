#ifndef __HWAVWRITER_H
#define __HWAVWRITER_H

template <class T>
class HWavWriter : public HWav, public HFileWriter<T>
{
    private:

        size_t _size;

    public:

        HWavWriter(const char* filename, H_SAMPLE_FORMAT format, int channels, H_SAMPLE_RATE rate, size_t size):
            HWav(filename, format, channels, rate, size),
            HFileWriter<T>(filename),
            _size(0)
        {}

        bool Start(void* unused);

        bool Stop();

        virtual int Write(T* dest, size_t blocksize);
};

/********************************************************************
Class implementation
********************************************************************/

template <class T>
bool HWavWriter<T>::Start(void* unused)
{
    // We know that the header is _always_ 44 bytes long for any wav
    // file, regardless of format, rate and number of channels. So we
    // can safely divide 44 by the size of the datatype (1, 2, 4) to get
    // the correct number of bytes to write
    HLog("Writing header");
    return HFileWriter<T>::Write((T*) &_header, 44 / sizeof(T)) == 44 / sizeof(T);
}

template <class T>
bool HWavWriter<T>::Stop()
{
    // Rewind and update size fields
    HLog("Calculate final size, %d blocks has been written", _size);
    // Todo: Update header
    HLog("Updating header");
    return true;
}

template <class T>
int HWavWriter<T>::Write(T* dest, size_t blocksize)
{
    // Do note that size contains the size in blocks, eg. number of samples,
    // not the final size in bytes..!
    _size += blocksize;
    return HFileWriter<T>::Write(dest, blocksize);
}
#endif