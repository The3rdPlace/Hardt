#ifndef __HCHUNKWRITER_H
#define __HCHUNKWRITER_H

/**
    Takes a block of samples and writes it to the next writer in smaller
    chunks.

    The chunksize must be an integral part of the incomming blocksize.
    Ex: Write(data, 4096)  ==>  Write(data, 1024) + Write(data+1024, 1024) + ...
*/
template <class T>
class HChunkWriter : public HWriter<T>
{
    private:

        int _chunksize;

    public:

        int Write(T* src, size_t blocksize);
        virtual int WriteChunk(T* src, size_t chunksize) = 0;

        HChunkWriter():
            _chunksize(0)
        {}

        HChunkWriter(int chunksize):
            _chunksize(chunksize)
        {}

        int GetChunksize()
        {
            return _chunksize;
        }
};

#endif