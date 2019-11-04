#ifndef __HWRITER_H
#define __HWRITER_H

/**
    Base class for all writer types
*/
template <class T>
class HWriter : public HObject
{
    public:

        /** Virtual destructor enabling delete on derived objects */
        virtual ~HWriter() = default;

        /** Write a block of samples */
        virtual int Write(T* src, size_t blocksize) = 0;

        /** Initialize before first write */
        virtual bool Start()
        {
            return true;
        }

        /** Clean after last write */
        virtual bool Stop()
        {
            return true;
        }

        /** Return a pointer to this instance as a HWriter* */
        HWriter<T>* Writer()
        {
            return (HWriter<T>*) this;
        }
};

#endif