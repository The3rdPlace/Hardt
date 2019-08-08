#ifndef __HLINEARMIXER_H
#define __HLINEARMIXER_H

/**
    Linear mixer that simply mixes (without level adjustment) two signals.
*/
template <class T>
class HLinearMixer : public HReader<T>
{
    private:

        HReader<T>* _reader_1;
        HReader<T>* _reader_2;

        int _blocksize;
        T* _buffer_1;
        T* _buffer_2;

    public:

        /** Construt a new HLinearMixer */
        HLinearMixer(HReader<T>* reader_1, HReader<T>* reader_2, size_t blocksize);

        /** Default destructor */
        ~HLinearMixer();

        /* Read a block of samples */
        int Read(T* dest, size_t blocksize);

        /** Initialize before first read */
        bool Start();

        /** Cleanup after last read */
        bool Stop();
};

#endif