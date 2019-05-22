#ifndef __HPROCESSOR_H
#define __HPROCESSOR_H

#include "hwriter.h"
#include "hreader.h"

template <class T>
class HProcessor : public HObject
{
    private:

        HWriter<T>* _writer;
        HReader<T>* _reader;
        T* _buffer;
        int _blocksize;
        bool* _terminated;

    private:

        bool Start();

        bool Stop();

    protected:

        HProcessor(HWriter<T>* writer, HReader<T>* reader, size_t blocksize, bool* terminationToken);

        ~HProcessor();

    public:

        int Read(T* dest, int blocksize);

        int Write(T* src, int blocksize);

        void SetReader(HReader<T>* reader);

        void SetWriter(HWriter<T>* writer);

        HReader<T>* GetReader();

        HWriter<T>* GetWriter();

        virtual void Run(long unsigned int blocks = 0);

        virtual void Halt();
};

#endif