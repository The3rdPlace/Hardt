#ifndef __HFILEWRITER_H
#define __HFILEWRITER_H

template <class T>
class HFileWriter : public HWriter<T>
{
    private:

        std::ofstream _stream;
        int _blocksize;
        const char* _filename;

    public:

        HFileWriter(const char* filename, int blocksize = 1024);
        ~HFileWriter();
        int Write(T* dest);
        int Blocksize();
};

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HFileWriter<T>::HFileWriter(const char* filename, int blocksize):
    _blocksize(blocksize),
    _filename(filename)
{
    _stream.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    if( !_stream.is_open())
    {
        HError("Failed to open file %s", filename);
        throw new HFileIOException("Failed to open file");
    }
}

template <class T>
HFileWriter<T>::~HFileWriter()
{
    _stream.close();
}

template <class T>
int HFileWriter<T>::Write(T* src)
{
    _stream.write((char*) src, _blocksize * sizeof(T));
    return _blocksize;
}

template <class T>
int HFileWriter<T>::Blocksize()
{
    return _blocksize;
}

#endif
