#ifndef __HFILEWRITER_H
#define __HFILEWRITER_H

template <class T>
class HFileWriter : public HWriter<T>
{
    private:

        std::ofstream _stream;
        const char* _filename;

    public:

        HFileWriter(const char* filename);
        ~HFileWriter();
        virtual int Write(T* dest, size_t blocksize);

        void Seek(int bytes);
};

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HFileWriter<T>::HFileWriter(const char* filename):
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
int HFileWriter<T>::Write(T* src, size_t blocksize)
{
    _stream.write((char*) src, blocksize * sizeof(T));
    return blocksize;
}

template <class T>
void HFileWriter<T>::Seek(int bytes)
{
    _stream.seekp(bytes, std::ios::beg);
}

#endif
