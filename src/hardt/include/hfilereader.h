#ifndef __HFILEREADER_H
#define __HFILEREADER_H

template <class T>
class HFileReader : public HReader<T>
{
    private:

        std::ifstream _stream;
        const char* _filename;

    public:

        HFileReader(const char* filename);
        virtual int Read(T* dest, size_t blocksize);
        bool Start(void* data);
        bool Stop();

        void Seek(int bytes);
};

#endif