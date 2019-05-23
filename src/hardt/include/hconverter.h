#ifndef __HCONVERTER_H
#define __HCONVERTER_H

template <class T>
class HConverter : public HWriter<T>
{
    private:

        void (*_readyCallback)();

    protected:

        void Ready();

    public:

        HConverter(void (*readyCallback)());

        int Write(T* src, size_t blocksize);

        virtual int Convert(T* src, size_t blocksize) = 0;
};

#endif