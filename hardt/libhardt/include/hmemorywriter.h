#ifndef __HMEMORYWRITER_H
#define __HMEMORYWRITER_H

/**
    Write into a fixed buffer

    Will write a finite number of samples to a fixed buffer, then return
    0 on subsequent writes.

    Most usefull while your are building or testing an application and
    need to write a single block of samples to a buffer for examination
    or custom handling.
*/
template <class T>
class HMemoryWriter : public HWriter<T>, public HWriterConsumer<T>
{
    private:

        T* _buffer;
        int _size;
        int _pos;
        bool _infinite;

    public:

        /** Construct a new HMemoryWriter */
        HMemoryWriter(T* buffer, size_t size, bool infinite = false):
            _buffer(buffer),
            _size(size),
            _pos(0),
            _infinite(infinite)
        {}

        /** Construct a new HMemoryWriter that registers with the upstream writer */
        HMemoryWriter(HWriterConsumer<T>* consumer, T* buffer, size_t size, bool infinite = false):
            _buffer(buffer),
            _size(size),
            _pos(0),
            _infinite(infinite)
        {
            consumer->SetWriter(this);
        }

        /** Write a block of samples */
        int Write(T* src, size_t blocksize)
        {
            if( _pos + blocksize > _size ) {
                return 0;
            }
            memcpy((void*) &_buffer[_pos], (void*) src, blocksize * sizeof(T));
            _pos += _infinite ? 0 : blocksize;
            return blocksize;
        }

        /** Implements HWriterConsumer::SetWriter() */
        void SetWriter(HWriter<T>* writer)
        {
            // Not used, writes into a buffer
        }

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            // No further propagation of commands
            return true;
        }

        /** Reset the buffer position so next write is to the start of the buffer */
        void Reset() {
            _pos = 0;
        }

        /** Get the current buffer position (index of next write start) */
        int GetPosition() {
            return _pos;
        }

        /** Get the size of the buffer */
        int GetSize() {
            return _size;
        }
};

#endif