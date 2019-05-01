#ifndef __HPROCESSOR_H
#define __HPROCESSOR_H

#include "hwriter.h"
#include "hreader.h"

template <class T>
class HProcessor : public HObject
{
    protected:

        HWriter<T>* _writer;
        HReader<T>* _reader;

    public:

        HProcessor(HWriter<T>* writer, HReader<T>* reader);

        int Read(T* dest, int blocksize);

        int Write(T* src, int blocksize);

        bool Start(void* data);

        bool Stop();

        void SetReader(HReader<T>* reader);

        void SetWriter(HWriter<T>* writer);

        virtual void Run() = 0;
};

#endif