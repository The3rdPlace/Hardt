#ifndef __HPROCESSOR_H
#define __HPROCESSOR_H

#include "hwriter.h"
#include "hreader.h"

/**
    The processor makes a Hardt application tick, it reads from a given reader
    component and writes the data to a writer class.

    This allows you to split your DSP chain into one or more readers, and
    one or more writers. Readers sort of 'pull' data from reader endpoints and writers
    'push' data to writer endpoints (typical a HOutput type), but it can also be a
    HNullWriter if you dont need to have the output.

    When you have created your readers and writers, then create a processor with the
    last reader and first writer and then call Run() on the processor.

    A processor will stop when it receives a zero-length read or if you call the Halt() method,
    or the bool variable pointed to by the 'terminationToken' parameter of the constructor is
    set to true (especially usefull if you have multiple threads or does signal handling).
*/
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

        /**
            Read directly from the associated reader
        */
        int Read(T* dest, int blocksize);

        /**
            Write directly to the associated writer
        */
        int Write(T* src, int blocksize);

        /**
            Set a (new) reader
        */
        void SetReader(HReader<T>* reader);

        /**
            Set a (new) writer
        */
        void SetWriter(HWriter<T>* writer);

        /**
            Get the associated reader
        */
        HReader<T>* GetReader();

        /**
            Get the associated writer
        */
        HWriter<T>* GetWriter();

        /**
            Read from the reader and write to the writer.

            Processing stops when a zero-length read is received,
            halt() is called or the associated terminationToken is set to true.
        */
        virtual void Run(long unsigned int blocks = 0);

        /**
            Stop processing.

            If the reader and writer is not destroyed, processing can be resumed
            by calling Run() once agan after setting the terminationToken to false
        */
        virtual void Halt();
};

#endif