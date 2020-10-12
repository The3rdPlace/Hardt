#ifndef __HMUX_H
#define __HMUX_H

#include "hreader.h"
#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hprobe.h"

/**
    Taking a list of readers, the read from each reader
    and interlace the samples - then return the resulting
    buffer. (or await writes from a number of writers, then multiplex
    the incomming data and write to the next writer)

    Ex: Having processed the left and right part of a stereo signal, a HMux object
    is then created with the left and right HReader objects. The HMux object will
    then read the left and right samples, interlace them and return a set of
    samples ready to output from a 2-channel soundcard.

    IQ samples having been processed in parallel branches may also require
    multiplexing again to end up with an IQ signal (for detection or other reasons).

    Do note that the output from the multiplexer will be the total number of samples
    read or written by the upstream readers or writers. Having two readers that each returns
    1024 samples will yield a read of 2048 samples.
*/
template <class T>
class HMux : public HReader<T>, public HWriter<T>, public HWriterConsumer<T>
{
    private:

        int _blocksize;

        std::vector< HReader<T>* > _readers;

        int _writers;
        HWriter<T>* _writer;
        int _received;
        T* _output;

        T** _buffers;
        int _bufferCount;

    public:

        /** Construct a new HMux */
        HMux( std::vector< HReader<T>* > readers, size_t blocksize);

        /** Construct a new HMux */
        HMux( HWriter<T>* writer, int writers, size_t blocksize);

        /** Construct a new HMux */
        HMux( std::vector< HWriterConsumer<T>* > consumers, size_t blocksize);

        /** Default destructor */
        ~HMux();

        /** Read a block of samples */
        int Read(T* dest, size_t blocksize);

        /** Write a block of samples */
        int Write(T* src, size_t blocksize);

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            if( _readers.size() != 0 ) {
                for (int i = 0; i < _readers.size(); i++) {
                    if (!_readers[i]->Command(command)) {
                        return false;
                    }
                }
                return true;
            }
            else if( _writer != nullptr ) {
                return _writer->Command(command);
            }
            return true;
        }

        /** Implements HWriterConsumer::SetWriter() */
        void SetWriter(HWriter<T>* writer)
        {
            _writer = writer;
        }
};

#endif