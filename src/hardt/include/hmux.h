#ifndef __HMUX_H
#define __HMUX_H

template <class T>
class HMux : public HReader<T>
{
    private:

        std::vector< HReader<T>* > _readers;
        T** _buffers;

    public:

        HMux( std::vector< HReader<T>* > readers, size_t blocksize);

        ~HMux();

        int Read(T* dest, size_t blocksize);
};

#endif