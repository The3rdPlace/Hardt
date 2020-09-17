#ifndef __HCONVERTER_H
#define __HCONVERTER_H

#include "hprobe.h"

/**
    Base class for Converters that convert one type to another.

    This class can not be instantiated directly but must be
    inherited and the pure virtual function Convert() implemented.
*/
template <class Tin, class Tout>
class HConverter : public HReader<Tout>, public HWriter<Tin>, public HWriterConsumer<Tout>
{
    private:

        HReader<Tin>* _reader;
        HWriter<Tout>* _writer;
        size_t _blocksizeIn;
        size_t _blocksizeOut;

        HProbe<Tout>* _probe;

        Tin *_input;
        Tout *_output;

    public:

        /**
         * Initialize before first read or write
         */
        bool Start()
        {
            return (_reader != NULL ? _reader->Start() : true) && (_writer != NULL ? _writer->Start() : true);
        }

        /**
         * Cleanup after last read or write
         */
        bool Stop()
        {
            return (_reader != NULL ?_reader->Stop() : true) && (_writer != NULL ?_writer->Stop() : true);
        }

    protected:

        /**
         * Construct a new HConverter that reads from a reader
         *
         * @param reader Upstream reader
         * @param blocksizeIn Number of input samples
         * @param blocksizeOut Number of output samples
         * @param probe Probe
         */
        HConverter(HReader<Tin>* reader, size_t blocksizeIn, size_t blocksizeOut, HProbe<Tout>* probe = nullptr):
            _reader(reader),
            _writer(nullptr),
            _blocksizeIn(blocksizeIn),
            _blocksizeOut(blocksizeOut),
            _output(NULL),
            _probe(probe) {

            _input = new Tin[blocksizeIn];
        }

        /**
         * Construct a new HConverter that writes to a writer
         *
         * @param writer Downstream writer
         * @param blocksizeIn Number of input samples
         * @param blocksizeOut Number of output samples
         * @param probe Probe
         */
        HConverter(HWriter<Tout>* writer, size_t blocksizeIn, size_t blocksizeOut, HProbe<Tout>* probe = nullptr):
            _reader(nullptr),
            _writer(writer),
            _blocksizeIn(blocksizeIn),
            _blocksizeOut(blocksizeOut),
            _input(NULL),
            _probe(probe) {

            _output = new Tout[blocksizeOut];
        }

        /**
         * Construct a new HConverter that registers with an upstream writer
         *
         * @param consumer Upstream consumer
         * @param blocksizeIn Number of input samples
         * @param blocksizeOut Number of output samples
         * @param probe Probe
         */
        HConverter(HWriterConsumer<Tin>* consumer, size_t blocksizeIn, size_t blocksizeOut, HProbe<Tout>* probe = nullptr):
            _blocksizeIn(blocksizeIn),
            _blocksizeOut(blocksizeOut),
            _input(NULL),
            _probe(probe) {

            _output = new Tout[blocksizeOut];
            consumer->SetWriter(this->Writer());
        }

    public:

        /**
         * Default destructor
         */
        ~HConverter()
        {
            if( _input != NULL )
            {
                delete[] _input;
            }
            if( _output != NULL )
            {
                delete[] _output;
            }
        }

        /**
         * Read a block of converted samples
         *
         * @param dest Destination buffer
         * @param blocksize Number of samples to read
         * @returns Number of samples read
         */
        int Read(Tout* dest, size_t blocksize) {
            if( blocksize != _blocksizeOut )
            {
                HError("Request for conversion-read with blocksize %d expected %d blocks", blocksize, _blocksizeOut);
                throw new HConverterIOException("Invalid number of blocks");
            }

            int read = _reader->Read(_input, _blocksizeIn);
            if( read == 0 )
            {
                HLog("Zero length read. Returning eof (zero)");
                return 0;
            }
            if( read != _blocksizeIn )
            {
                HError("Request for read with blocksize %d returned %d blocks", _blocksizeIn, read);
                throw new HConverterIOException("Invalid number of blocks");
            }

            int converted;
            if( (converted = Convert(_input, dest, _blocksizeIn)) != _blocksizeOut )
            {
                HError("Request for conversion of blocksize %d blocks converted %d blocks", _blocksizeIn, converted);
                throw new HConverterIOException("Converted incorrect number of blocks");
            }

            if( _probe != nullptr ) {
                _probe->Write(dest, converted);
            }

            return _blocksizeOut;
        };

        /**
         * Write a block of samples to convert
         *
         * @param src Source buffer
         * @param blocksize Number of input samples
         * @returns Number of samples written
         */
        int Write(Tin* src, size_t blocksize) {
            if( blocksize != _blocksizeIn )
            {
                HError("Request for conversion-write with blocksize %d expected %d blocks", blocksize, _blocksizeIn);
                throw new HConverterIOException("Read incorrect number of blocks from input reader");
            }

            int converted;
            if( (converted = Convert(src, _output, _blocksizeIn)) != _blocksizeOut )
            {
                HError("Request for conversion of blocksize %d blocks converted %d blocks", _blocksizeIn, converted);
                throw new HConverterIOException("Converted incorrect number of blocks");
            }

            int written;
            if( (written = _writer->Write(_output, _blocksizeOut)) != _blocksizeOut )
            {
                HError("Request for write with blocksize %d wrote %d blocks", _blocksizeOut, written);
                throw new HConverterIOException("Wrote incorrect number of blocks to output writer");
            }

            if( _probe != nullptr ) {
                _probe->Write(_output, converted);
            }

            return _blocksizeIn;
        }

        /**
         * Convert a block of samples
         */
        virtual int Convert(Tin* src, Tout* dest, size_t blocksize) = 0;

        /**
         * Implements HWriterConsumer::SetWriter()
         */
        void SetWriter(HWriter<Tout>* writer)
        {
            HLog("setwriter %p", writer);
            _writer = writer;
        }

        /**
         * Transport a command to the next/previous component in the chain
         */
        bool Command(HCommand* command) {
            if( _writer != nullptr ) {
                return _writer->Command(command);
            } else if( _reader != nullptr ){
                return _reader->Command(command);
            }
            return true;
        }
};

#endif