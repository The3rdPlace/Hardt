#ifndef __HIQTRANSLATEBYTWO_H
#define __HIQTRANSLATEBYTWO_H

#include "hfilter.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hreader.h"
#include "hprobe.h"

/**
    Translating the input samples by two.

    This implements the common frequency translation method
    of translating by two (input * [1, -1, 1, -1, .... ])

    The result is that the spectrum is inversed, especially
    usefull when doing decimation or other types of frequency
    translation whereby the usefull spectrum ends up inverted.
*/
template <class T>
class HIqTranslateByTwo : public HFilter<T>
{
    private:

    public:

        /** Construct a new HIqTranslateByTwo object that writes to a writer */
        HIqTranslateByTwo(HWriter<T>* writer, size_t blocksize, HProbe<T>* probe = NULL):
            HFilter<T>(writer, blocksize, probe) {}

        /** Construct a new HIqTranslateByTwo object that registers with an upstream writer */
        HIqTranslateByTwo(HWriterConsumer<T>* consumer, size_t blocksize, HProbe<T>* probe = NULL):
            HFilter<T>(consumer, blocksize, probe) {}

        /** Construct a new HIqTranslateByTwo object that reads from a reader */
        HIqTranslateByTwo(HReader<T>* reader, size_t blocksize, HProbe<T>* probe = NULL):
            HFilter<T>(reader, blocksize, probe) {}

        /** Default destructor */
        ~HIqTranslateByTwo() {}

        /** Run a block of samples through the translator */
        void Filter(T* src, T* dest, size_t blocksize);
};

#endif