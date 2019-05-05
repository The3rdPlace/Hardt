#ifndef __HREADER_H
#define __HREADER_H

template <class T>
class HReader : public HObject
{
    public:

        virtual int Read(T* dest, size_t blocksize) = 0;

        virtual bool Start(void* data);
        virtual bool Stop();
};

#endif