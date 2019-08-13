#ifndef __HWAVREADER_H
#define __HWAVREADER_H

/**
    Read samples from a wav file
*/
template <class T>
class HWavReader : public HWav, public HFileReader<T>
{
    public:

        /** Construct a new wav reader stream that reads the specified wav file */
        HWavReader(const char* filename);

        /** Initialize before first read */
        bool Start();
};

#endif