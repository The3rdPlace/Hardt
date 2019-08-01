#ifndef __HMUX_H
#define __HMUX_H

/**
    Taking a list of readers, the read from each reader
    and interlace the samples - then return the resulting
    buffer.

    Ex: Having processed the left and right part of a stereo signal, a HMux object
    is then created with the left and right HReader objects. The HMux object will
    then read the left and right samples, interlace them and return a set of
    samples ready to output from a 2-channel soundcard.

    The number of samples read from each reader will be half the number of
    samples returned in a Read() in the above example.
*/
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