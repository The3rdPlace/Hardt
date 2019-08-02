#ifndef __HNULLWRITER_H
#define __HNULLWRITER_H

/**
    /dev/null

    Accepts any number of writes of any blocksize
    and throws the data away.

    Most usefull while your are building or testing an application and
    dont want to spend time finishing a branch that has no relation to your
    current focus area.
*/
template <class T>
class HNullWriter : public HWriter<T>
{
    public:

        int Write(T* src, size_t blocksize)
        {
            return blocksize;
        }
};

#endif