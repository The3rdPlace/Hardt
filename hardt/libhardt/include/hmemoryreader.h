#ifndef __HMEMORYREADER_H
#define __HMEMORYREADER_H

/**
    Read from a fixed buffer

    Will read a finite number of samples to from a fixed buffer, then return
    0 on subsequent reads.

    Most usefull while your are building or testing an application and
    need to read a single block of samples from a fixed bufer
*/
template <class T>
class HMemoryReader : public HReader<T>
{
    private:

        T* _buffer;
        int _size;
        int _pos;

    public:

        /** Construct a new HMemoryReader */
        HMemoryReader(T* buffer, size_t size):
            _buffer(buffer),
            _size(size),
            _pos(0)
        {}

        /** Read a block of samples */
        int Read(T* dest, size_t blocksize)
        {
            if( _pos + blocksize > _size ) {
                return 0;
            }
            memcpy((void*) dest, (void*) &_buffer[_pos], blocksize * sizeof(T));
            _pos += blocksize;
            return blocksize;
        }

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            // No further propagation of commands
            return true;
        }

        /** Reset the buffer position so next read is from the start of the buffer */
        void Reset() {
            _pos = 0;
        }

        /** Get the current buffer position (index of next read start) */
        int GetPosition() {
            return _pos;
        }

        /** Get the size of the buffer */
        int GetSize() {
            return _size;
        }
};

#endif