#ifndef __HFILTERBASE_H
#define __HFILTERBASE_H

template <class T>
class HFilterBase
{
    public:

        virtual void Filter(T* src, T* dest, size_t blocksize) = 0;
};

#endif