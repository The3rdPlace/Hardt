#ifndef __HIQ2QCONVERTER_H
#define __HIQ2QCONVERTER_H

#include "hdecimator.h"

/**
 * Convert an incomming IQ data block to a block containing only the Q channel.
 *
 * The conversion collects two blocks and will thus retain the given blocksize
 * in the write to the downstream component, or do two reads from the upstream
 * reader before returning.
 *
 * @note This is merely a wrapper around a HDecimator with a decimation factor of 2
 *       and a start offset in the incomming IQ block at index 1
 */
template <class T>
class HIq2QConverter : public HDecimator<T> {

public:

    /**
     * Construct a new HIq2IConverter that handle writers.
     *
     * @param id Element identifier
     * @param writer The downstream writer
     * @param blocksize The expected input and output blocksize
     * @param probe Probe
     */
    HIq2QConverter(std::string id, HWriter<T>* writer, size_t blocksize, HProbe<T>* probe = nullptr):
            HDecimator<T>(id, writer, 2, blocksize, true, 1, probe) {}

    /**
     * Construct a new HIq2IConverter that handle writer consumers.
     *
     * @param id Element identifier
     * @param consumer The upstream consumer to receive this as a writer
     * @param blocksize The expected input and output blocksize
     * @param probe Probe
     */
    HIq2QConverter(std::string id, HWriterConsumer<T>* consumer, size_t blocksize, HProbe<T>* probe = nullptr):
            HDecimator<T>(id, consumer, 2, blocksize, true, 1, probe) {}

    /**
     * Construct a new HIq2IConverter that handle readers.
     *
     * @param id Element identifier
     * @param reader The upstream reader
     * @param blocksize The expected input and output blocksize
     * @param probe Probe
     */
    HIq2QConverter(std::string id, HReader<T>* reader, size_t blocksize, HProbe<T>* probe = nullptr):
            HDecimator<T>(id, reader, 2, blocksize, true, 1, probe) {}

};

#endif