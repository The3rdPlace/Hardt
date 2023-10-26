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

        // Prevent instantiation of interpolator through this wrapper class
        HUpsampler(std::string id, HWriter<T>* writer, int factor, float* coefficients, int length, size_t blocksize, HProbe<T>* probe = nullptr):
            HInterpolator<T>(id, writer, factor, coefficients, length, blocksize, probe) {}

        // Prevent instantiation of interpolator through this wrapper class
        HUpsampler(std::string id, HWriterConsumer<T>* consumer, int factor, float* coefficients, int length, size_t blocksize, HProbe<T>* probe = nullptr):
            HInterpolator<T>(id, consumer, factor, coefficients, length, blocksize, probe) {}

        // Prevent instantiation of interpolator through this  wrapper class
        HUpsampler(std::string id, HReader<T>* reader, int factor, float* coefficients, int length, size_t blocksize, HProbe<T>* probe = nullptr):
            HInterpolator<T>(id, reader, factor, coefficients, length, blocksize, probe) {}

    public:

        /**
         * Construct a new HUpsampler that handle writers.
         *
         * @param writer The downstream writer
         * @param factor Interpolation factor, 1 or larger
         * @param blocksize The expected input and output blocksize
         * @param probe Probe
         */
        HUpsampler(std::string id, HWriter<T>* writer, int factor, size_t blocksize, HProbe<T>* probe = nullptr):
            HInterpolator<T>(id, writer, factor, nullptr, 0, blocksize, probe) {}

        /**
         * Construct a new HUpsampler that handle writer consumers.
         *
         * @param consumer The upstream consumer to receive this as a writer
         * @param factor Interpolation factor, 1 or larger
         * @param blocksize The expected input and output blocksize
         * @param probe Probe
         */
        HUpsampler(std::string id, HWriterConsumer<T>* consumer, int factor, size_t blocksize, HProbe<T>* probe = nullptr):
            HInterpolator<T>(id, consumer, factor, nullptr, 0, blocksize, probe) {}

        /**
         * Construct a new HUpsampler that handle readers.
         *
         * @param reader The upstream reader
         * @param factor Interpolation factor, 1 or larger
         * @param blocksize The expected input and output blocksize
         * @param probe Probe
         */
        HUpsampler(std::string id, HReader<T>* reader, int factor, size_t blocksize, HProbe<T>* probe = nullptr):
            HInterpolator<T>(id, reader, factor, nullptr, 0, blocksize, probe) {}
};

#endif
