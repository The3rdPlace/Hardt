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

        /** Write a block of samples */
        int Write(T* src, size_t blocksize);

        /** Write a chunk of samples.
            This function must be implemented by classes inheriting the HChunkWriter class */
        virtual int WriteChunk(T* src, size_t chunksize) = 0;

        /** Construct a chunkwriter with chunk size 0 (zero). This will write
            all blocks straight through without chunking */
        HChunkWriter():
            _chunksize(0)
        {}

        /** Construct a chunkwriter with the given chunk size */
        HChunkWriter(int chunksize):
            _chunksize(chunksize)
        {}

        /** Get the chunk size used by this writer */
        int GetChunksize()
        {
            return _chunksize;
        }
};

#endif