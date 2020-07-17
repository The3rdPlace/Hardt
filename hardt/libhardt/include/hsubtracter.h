#ifndef __HSUBTRACTER_H
#define __HSUBTRACTER_H

/**
    Linear mixer that subtracts signal 2 from signal 1.
*/
template <class T>
class HSubtracter : public HReader<T>, public HWriter<T>, public HWriterConsumer<T>
{
    private:

        HReader<T>* _reader_1;
        HReader<T>* _reader_2;
        HWriter<T>* _writer;
        bool _wait;

        int _blocksize;
        T* _buffer_1;
        T* _buffer_2;

        HProbe<T>* _probe;

    public:

        /** Construct a new HSubtracter */
        HSubtracter(HReader<T>* reader_1, HReader<T>* reader_2, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HSubtracter */
        HSubtracter(HReader<T>* reader, HWriter<T>* writer, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HSubtracter which mixes two inputs by multiplexing between alternating writes.
            Every 1st. write is kept in a buffer and with the 2nd. write, the sample values from the first and
            second write is subtracted. The result is then written to the next writer.
         */
        HSubtracter(HWriter<T>* writer, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HSubtracter */
        HSubtracter(HReader<T>* reader, HWriterConsumer<T>* consumer, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HSubtracter */
        HSubtracter(HWriterConsumer<T>* consumer, size_t blocksize, HProbe<T>* probe = NULL);

        /** Default destructor */
        ~HSubtracter();

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

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            if( _writer != nullptr )
            {
                if (!_writer->Command(command)) {
                    return false;
                }
            }
            if( _reader_1 != nullptr )
            {
                if (!_reader_1->Command(command)) {
                    return false;
                }
            }
            if( _reader_2 != nullptr )
            {
                if( !_reader_2->Command(command) ) {
                    return false;
                }
            }
            return true;
        }
};

#endif