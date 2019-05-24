#ifndef __HNULLWRITER_H
#define __HNULLWRITER_H

template <class T>
class HNullWriter : public HWriter<T>
{
    public:

        int Write(T* src, size_t blocksize);
};

#endif