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
        virtual int Write(T* dest, size_t blocksize);
        bool Start(void* data);
        bool Stop();

        void Seek(int bytes);
};

/********************************************************************
Class implementation
********************************************************************/

template <class T>
HFileWriter<T>::HFileWriter(const char* filename):
    _filename(filename)
{}

template <class T>
int HFileWriter<T>::Write(T* src, size_t blocksize)
{
    this->Metrics.Writes++;
    _stream.write((char*) src, blocksize * sizeof(T));
    this->Metrics.BlocksOut += blocksize;
    this->Metrics.BytesOut += blocksize * sizeof(T);
    return blocksize;
}

template <class T>
void HFileWriter<T>::Seek(int bytes)
{
    _stream.seekp(bytes, std::ios::beg);
}

template <class T>
bool HFileWriter<T>::Start(void* data)
{
    HLog("Trying to open stream for %s", _filename);
    _stream.open(_filename, std::ios::out | std::ios::binary | std::ios::trunc);
    if( !_stream.is_open())
    {
        HError("Failed to open file %s", _filename);
        return false;
    }
    HLog("Stream is open");
    return true;
}

template <class T>
bool HFileWriter<T>::Stop()
{
    HLog("Closing stream");
    _stream.close();
    return true;
}

#endif
