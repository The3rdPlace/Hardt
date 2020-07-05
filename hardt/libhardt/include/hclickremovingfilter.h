#ifndef __HCLICKREMOVINGFILTER_H
#define __HCLICKREMOVINGFILTER_H

#include "hardt.h"
#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hprobe.h"

/**
    A filter providing click removal by changing samples that exceed
    the average sample value by 50% to 0
*/
template <class T>
class HClickRemovingFilter : public HFilter<T>
{
    public:

        /** Construct a new HClickRemovingFilter object that writes to a writer */
        HClickRemovingFilter(HWriter<T>* writer, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HClickRemovingFilter object that registers with an upstream writer */
        HClickRemovingFilter(HWriterConsumer<T>* consumer, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HClickRemovingFilter object that reads from a reader */
        HClickRemovingFilter(HReader<T>* reader, size_t blocksize, HProbe<T>* probe = NULL);

        /** Default destructor */
        ~HClickRemovingFilter();

        /** Run a block of samples through the filter */
        virtual void Filter(T* src, T* dest, size_t blocksize);
};

#endif