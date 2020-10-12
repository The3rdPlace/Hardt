#ifndef __HWAVREADER_H
#define __HWAVREADER_H

#include "hprobe.h"

/**
    Read samples from a wav file
*/
template <class T>
class HWavReader : public HWav, public HFileReader<T>
{
    public:

        /** Construct a new wav reader stream that reads the specified wav file */
        HWavReader(const char* filename, HProbe<T>* probe = nullptr);

        /** Initialize before first read */
        bool Start();

        /** Execute and/or pass on a command */
        bool Command(HCommand* command) {
            // No further propagation of commands
            return true;
        }
};

#endif