#ifndef __HCONVERTER_H
#define __HCONVERTER_H

template <class InT, class OutT>
class HConverter : public HWriter<InT>
{
    virtual int Write(InT* src, size_t blocksize) = 0;
};

#endif