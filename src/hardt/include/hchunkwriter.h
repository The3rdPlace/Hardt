#ifndef __HCHUNKWRITER_H
#define __HCHUNKWRITER_H

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