#ifndef __HSTREAMPROCESSOR_H
#define __HSTREAMPROCESSOR_H

#include "hreader.h"
#include "hwriter.h"

template <class T>
class HStreamProcessor : public HProcessor<T>
{
    private:

        HReader<T>* _reader;
        HWriter<T>* _writer;

    public:

        HStreamProcessor(HWriter<T>* writer, HReader<T>* reader, int blocksize, bool* terminationToken);

        void Run(long unsigned int blocks = 0);
};

#endif