#ifndef __HNULLREADER_H
#define __HNULLREADER_H

#include <unistd.h>

/**
    Read silence
*/
template <class T>
class HNullReader : public HReader<T>
{
    private:

        T _zero;

    public:

        /** Create a new HNullReader object */
        HNullReader()
        {
            _zero = std::numeric_limits<T>::lowest();
        }

        /** Read a block of silence */
        int Read(T* dest, size_t blocksize);
};

#endif
