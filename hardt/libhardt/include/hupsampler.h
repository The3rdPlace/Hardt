#ifndef __HUPSAMPLER_H
#define __HUPSAMPLER_H

#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hprobe.h"
#include "hinterpolator.h"

#include <vector>

/** Impements an upsampler that can increase the incomming samplerate by a given factor */
template <class T>
class HUpsampler: public HInterpolator<T>
{
    private:

        // Prevent instantiation of interpolator
        HUpsampler(HWriter<T>* writer, int factor, float* coefficients, int length, size_t blocksize):
            HInterpolator<T>(writer, factor, coefficients, length, blocksize) {}

        // Prevent instantiation of interpolator
        HUpsampler(HWriterConsumer<T>* consumer, int factor, float* coefficients, int length, size_t blocksize):
            HInterpolator<T>(consumer, factor, coefficients, length, blocksize) {}

        // Prevent instantiation of interpolator
        HUpsampler(HReader<T>* reader, int factor, float* coefficients, int length, size_t blocksize):
            HInterpolator<T>(reader, factor, coefficients, length, blocksize) {}

    public:

        /** Construct a new HUpsampler that handle writers.

            Parameters:
              writer = The downstream writer
              factor = Interpolation factor, 1 or larger
              blocksize = The expected input and output blocksize
         */
        HUpsampler(HWriter<T>* writer, int factor, size_t blocksize):
            HInterpolator<T>(writer, factor, nullptr, 0, blocksize) {}

        /** Construct a new HUpsampler that handle writer consumers.

            Parameters:
              consumer = The upstream consumer to receive this as a writer
              factor = Interpolation factor, 1 or larger
              blocksize = The expected input and output blocksize
         */
        HUpsampler(HWriterConsumer<T>* consumer, int factor, size_t blocksize):
            HInterpolator<T>(consumer, factor, nullptr, 0, blocksize) {}

        /** Construct a new HUpsampler that handle readers.

              reader = The upstream reader
              factor = Interpolation factor, 1 or larger
              blocksize = The expected input and output blocksize
         */
        HUpsampler(HReader<T>* reader, int factor, size_t blocksize):
            HInterpolator<T>(reader, factor, nullptr, 0, blocksize) {}
};

#endif
