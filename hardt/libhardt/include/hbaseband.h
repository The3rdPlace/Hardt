#ifndef __HBASEBAND_H
#define __HBASEBAND_H

#include "hfilter.h"
#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hprobe.h"

/**
    A filter that translates a segment in the spectrum down
    to baseband and filters away everything above the baseband segment
*/
template <class T>
class HBaseband : public HFilter<T>
{
    private:

        int _center;
        int _width;
        int _rate;

    public:

        /** Construct a new HBaseband filter that writes to a writer */
        HBaseband(HWriter<T>* writer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HBaseband filter that registers with an upstream writer */
        HBaseband(HWriterConsumer<T>* consumer, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HBaseband filter that reads from a reader */
        HBaseband(HReader<T>* reader, H_SAMPLE_RATE rate, int center, int width, size_t blocksize, HProbe<T>* probe = NULL);

        /** Default destructor */
        ~HBaseband();

        /** Run a block of samples through the gain filter */
        virtual void Filter(T* src, T* dest, size_t blocksize);

        void SetSegment(int center, int width) {
            _center = center;
            _width = width;
        }
};

#endif