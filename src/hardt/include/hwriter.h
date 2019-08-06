#ifndef __HWRITER_H
#define __HWRITER_H

/**
    Base class for all writer types
*/
template <class T>
class HWriter : public HObject
{
    public:

        virtual int Write(T* src, size_t blocksize) = 0;

        virtual bool Start()
        {
            return true;
        }

        virtual bool Stop()
        {
            return true;
        }

        HWriter<T>* Writer()
        {
            return (HWriter<T>*) this;
        }
};

#endif