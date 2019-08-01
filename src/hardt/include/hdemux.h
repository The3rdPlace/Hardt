#ifndef __HDEMUX_H
#define __HDEMUX_H

/**
    De-multiplex a stream of samples.
    The incomming samples are written round-robin to the writer given in the
    list when constructing the object.

    Ex: Having sampled a signal from 2 channels, writing the stream of
    samples from soundcard to an instanse of this class initiaized with
    two writers - would result in the two writers receaving either the
    left or the right samples.

    The number of samples per block written to the output writers will be
    half the number of incomming samples in the above example.
*/

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