#ifndef __HDECIMATOR_H
#define __HDECIMATOR_H

#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hprobe.h"

#include <vector>

/** Implements a decimator that can reduce the incomming samplerate by a given factor */
template <class T>
class HDecimator: public HReader<T>, public HWriter<T>, public HWriterConsumer<T>
{
    private:

        HWriter<T>* _writer;
        HReader<T>* _reader;
        size_t _blocksize;

        int _factor;
        T* _buffer;        
        size_t _length;

        void Init();

    public:

        /** Construct a new HDecimator that handle writers.

            Parameters:
              writer = The downstream writer
              factor = Decimation factor, 1 or larger
              blocksize = The expected input and output blocksize
         */
        HDecimator(HWriter<T>* writer, int factor, size_t blocksize);

        /** Construct a new HDecimator that handle writer consumers.

            Parameters:
              consumer = The upstream consumer to receive this as a writer
              factor = Decimation factor, 1 or larger
              blocksize = The expected input and output blocksize
         */
        HDecimator(HWriterConsumer<T>* consumer, int factor, size_t blocksize);

        /** Construct a new HDecimator that handle readers.

              reader = The upstream reader
              factor = Decimation factor, 1 or larger
              blocksize = The expected input and output blocksize
         */
        HDecimator(HReader<T>* reader, int factor, size_t blocksize);

        /** Implements HWriterConsumer::SetWriter() */
        void SetWriter(HWriter<T>* writer) {
            _writer = writer;
        }

        /** Default destructor */
        ~HDecimator();

        /** Write a block of samples */
        int Write(T* src, size_t blocksize);

        /** Read a block of samples */
        int Read(T* dest, size_t blocksize);

        /** Call Start() on up- or downstream components */
        bool Start() {
            if( _writer != nullptr )
            {
                return _writer->Start();
            }
            if( _reader != nullptr )
            {
                return _reader->Start();
            }
            return true;
        }

        /** Call Stop() on up- or downstream components */
        bool Stop() {
            if( _writer != nullptr )
            {
                return _writer->Stop();
            }
            if( _reader != nullptr )
            {
                return _reader->Stop();
            }
            return true;
        }

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            if( _writer != nullptr )
            {
                return _writer->Command(command);
            }
            if( _reader != nullptr )
            {
                return _reader->Command(command);
            }
            return true;
        }

        /** Set the decimation factor. Warning: This will reset the buffer position
            and start collecting samples for a write from position 0 (zero) */
        void SetFactor(int factor) {
            _factor = factor;
            _length = 0;
        }
};

#endif
