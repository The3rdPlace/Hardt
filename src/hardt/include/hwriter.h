#ifndef __HWRITER_H
#define __HWRITER_H

template <class T>
class HWriter : public HObject
{
    public:

        virtual int Write(T* src, size_t blocksize) = 0;

        virtual bool Start(void* data);
        virtual bool Stop();
};

#endif