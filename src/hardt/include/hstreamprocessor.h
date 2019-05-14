#ifndef __HSTREAMPROCESSOR_H
#define __HSTREAMPROCESSOR_H

#include "hreader.h"
#include "hwriter.h"

template <class T>
class HStreamProcessor : public HProcessor<T>
{
    private:

        bool* _terminated;
        HReader<T>* _reader;
        HWriter<T>* _writer;
        T* _buffer;
        int _blocksize;

    public:

        HStreamProcessor(HWriter<T>* writer, HReader<T>* reader, int blocksize, bool* terminationToken);

        ~HStreamProcessor();

        void Run();
        void Halt();
};

#endif