#ifndef __HLINEARMIXER_H
#define __HLINEARMIXER_H

template <class T>
class HLinearMixer : public HReader<T>
{
    private:

        HReader<T>* _reader_1;
        HReader<T>* _reader_2;

        int _blocksize;
        T* _buffer;

    public:

        HLinearMixer(HReader<T>* reader_1, HReader<T>* reader_2, size_t blocksize);

        ~HLinearMixer();

        int Read(T* dest, size_t blocksize);
};

#endif