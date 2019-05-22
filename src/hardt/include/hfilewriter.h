#ifndef __HFILEWRITERTMP_H
#define __HFILEWRITERTMP_H

#include <stdio.h>
#include <fstream>

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