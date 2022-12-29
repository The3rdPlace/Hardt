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

    A special use of the multiplexer is to have a single stream (reader og writer) and then
    multiplexing the input data into two identical channels. This is usefull when you want
    to output a mono stream as a dual-channel ("stereo") headset.
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

        bool _duplex;

    public:

        /**
         * Construct a new HMux
         *
         * @param readers A list o reader to read from
         * @param blocksize Number of blocks to read
         * @param duplex If set to true, the number of readers _must_ be 1. Read
         *               a single block, then duplicate the block and output same
         *               block as two channels multiplexed (mono->stereo for mono input)
         */
        HMux(std::string id, std::vector< HReader<T>* > readers, size_t blocksize, bool duplex = false);

        /**
         * Construct a new HMux
         *
         * @param writer The writer to write the final multiplexed signal to
         * @param writers Number of writers, writing to this multiplexer
         * @param blocksize Number of blocks to write
         * @param duplex If set to true, the number of writers _must_ be 1. When
         *               a single block is written, then duplicate the block and output same
         *               block as two channels multiplexed (mono->stereo for mono input)
         */
        HMux(std::string id, HWriter<T>* writer, int writers, size_t blocksize, bool duplex = false);

        /**
         * Construct a new HMux
         *
         * @param consumers The upstream consumers
         * @param blocksize Number of blocks to write
         * @param duplex If set to true, the number of upstream writers _must_ be 1. When
         *               a single block is written, then duplicate the block and output same
         *               block as two channels multiplexed (mono->stereo for mono input)
         */
        HMux(std::string id, std::vector< HWriterConsumer<T>* > consumers, size_t blocksize, bool duplex = false);

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

        /** Start the multiplexer */
        bool Start()
        {
            if( _writer != nullptr ) {
                return _writer->Start();
            }
            if( _readers.size() > 0 ) {
                for (int i = 0; i < _readers.size(); i++) {
                    if (!_readers[i]->Start()) {
                        return false;
                    }
                }
            }
            return true;
        }

        /** Stop the multiplexer */
        bool Stop()
        {
            if( _writer != nullptr ) {
                return _writer->Stop();
            }
            if( _readers.size() > 0 ) {
                for (int i = 0; i < _readers.size(); i++) {
                    if (!_readers[i]->Stop()) {
                        return false;
                    }
                }
            }
            return true;
        }
};

#endif