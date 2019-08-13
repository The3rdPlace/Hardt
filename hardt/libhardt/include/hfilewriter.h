#ifndef __HFILEWRITER_H
#define __HFILEWRITER_H

#include <stdio.h>
#include <fstream>

/**
    Write to a file
*/
template <class T>
class HFileWriter : public HWriter<T>
{
    private:

        std::ofstream _stream;
        const char* _filename;

    public:

        /** Construct a new HFileWriter with the given filename */
        HFileWriter(const char* filename);

        /** Construct a new HFileWriter with the given filename and register
            with the given consumer */
        HFileWriter(const char* filename, HWriterConsumer<T>* consumer);

        /** Write to the file */
        virtual int Write(T* src, size_t blocksize);

        /** Initialize before first write */
        bool Start();

        /** Cleanup after last write */
        bool Stop();

        /** Seek to this position in the file */
        void Seek(int bytes);
};

#endif