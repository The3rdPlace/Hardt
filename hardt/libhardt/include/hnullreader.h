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
        explicit HNullReader(std::string id):
            HReader<T>(id) {
            _zero = std::numeric_limits<T>::lowest();
        }

        /** Read a block of silence */
        int ReadImpl(T* dest, size_t blocksize);

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            // No further propagation of commands
            return true;
        }
};

#endif
