#ifndef __HDEMUX_H
#define __HDEMUX_H

template <class T>
class HDeMux : public HWriter<T>
{
    private:

        std::vector< HWriter<T>* > _writers;
        T** _buffers;

    public:

        HDeMux( std::vector< HWriter<T>* > writers, size_t blocksize);

        ~HDeMux();

        int Write(T* src, size_t blocksize);
};

#endif