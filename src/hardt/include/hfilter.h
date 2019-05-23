#ifndef __HFILTER_H
#define __HFILTER_H

template <class T>
class HFilter : public HWriter<T>, public HReader<T>
{
    private:

        HWriter<T>* _writer;
        HReader<T>* _reader;

        T* _buffer;

    public:

        HFilter(HWriter<T>* writer, size_t blocksize);
        HFilter(HReader<T>* reader, size_t blocksize);

        ~HFilter();

        int Write(T* src, size_t blocksize);
        int Read(T* dest, size_t blocksize);

        virtual void Filter(T* src, T* dest, size_t blocksize) = 0;
};

#endif