#ifndef __HLINEARMIXER_H
#define __HLINEARMIXER_H

/**
    Linear mixer that simply mixes (without level adjustment) two signals.
*/
template <class T>
class HLinearMixer : public HReader<T>, public HWriter<T>
{
    private:

        HReader<T>* _reader_1;
        HReader<T>* _reader_2;
        HWriter<T>* _writer;

        int _blocksize;
        T* _buffer_1;
        T* _buffer_2;

        HProbe<T>* _probe;

    public:

        /** Construct a new HLinearMixer */
        HLinearMixer(HReader<T>* reader_1, HReader<T>* reader_2, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HLinearMixer */
        HLinearMixer(HReader<T>* reader, HWriter<T>* writer, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HLinearMixer */
        HLinearMixer(HReader<T>* reader, HWriterConsumer<T>* consumer, size_t blocksize, HProbe<T>* probe = NULL);

        /** Default destructor */
        ~HLinearMixer();

        /** Common initialization */
        void Init(size_t blocksize);

        /** Read a block of samples */
        int Read(T* dest, size_t blocksize);

        /** Write a block of samples */
        int Write(T* src, size_t blocksize);

        /** Initialize before first read */
        bool Start();

        /** Cleanup after last read */
        bool Stop();

        /** Set writer */
        void SetWriter(HWriter<T>* writer)
        {
            _writer = writer;
        }
};

#endif