#ifndef __HSTREAMPROCESSOR_H
#define __HSTREAMPROCESSOR_H

#include "hreader.h"
#include "hwriter.h"

/**
    Read samples from a local reader and write them to a local writer
*/
template <class T>
class HStreamProcessor : public HProcessor<T>
{
    private:

        HReader<T>* _reader;
        HWriter<T>* _writer;

    public:

        /** Construct a new HStreamProcessor */
        HStreamProcessor(HWriter<T>* writer, HReader<T>* reader, int blocksize, bool* terminationToken);

        /** Construct a new HStreamProcessor which will have the writer registered by a later
            writer, following the HWriterConsumer scheme */
        HStreamProcessor(HReader<T>* reader, int blocksize, bool* terminationToken);

        /** Run the processor */
        void Run(long unsigned int blocks = 0);
};

#endif