#ifndef __HREADER_H
#define __HREADER_H

template <class T>
class HReader : public HObject
{
    public:

        virtual int Read(T* dest, size_t blocksize) = 0;

        virtual bool Start()
        {
            return true;
        }

        virtual bool Stop()
        {
            return true;
        }

        HReader<T>* Reader()
        {
            return (HReader<T>*) this;
        }
};

#define READER (HReader<T>*)

#endif