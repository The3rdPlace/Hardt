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

        HFileWriter(const char* filename);
        virtual int Write(T* src, size_t blocksize);
        bool Start();
        bool Stop();

        void Seek(int bytes);
};

#endif