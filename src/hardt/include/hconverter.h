#ifndef __HCONVERTER_H
#define __HCONVERTER_H

template <class T>
class HConverter : public HWriter<T>
{
    int Write(T* src, size_t blocksize);
};

#endif