#ifndef __HCONVERTER_H
#define __HCONVERTER_H

/**
    Base class for Converters that convert one type to another.

    This class can not be instantiated directly but must be
    inherited and the pure virtual function Convert() implemented.
*/
template <class Tin, class Tout>
class HConverter : public HReader<Tout>, HWriter<Tin>, HWriterConsumer<Tout>
{
    private:

        HReader<Tin>* _reader;
        HWriter<Tout>* _writer;
        size_t _blocksize;

        Tin *_input;
        Tout *_output;

    public:

        /** Initialize before first read or write */
        bool Start()
        {
            return (_reader != NULL ? _reader->Start() : true) && (_writer != NULL ? _writer->Start() : true);
        }

        /** Cleanup after last read or write */
        bool Stop()
        {
            return (_reader != NULL ?_reader->Stop() : true) && (_writer != NULL ?_writer->Stop() : true);
        }

    protected:

        /** Construct a new HConverter that reads from a reader */
        HConverter(HReader<Tin>* reader, size_t blocksize):
            _reader(reader),
            _writer(nullptr),
            _blocksize(blocksize),
            _output(NULL)
        {
            _input = new Tin[blocksize];
        }

        /** Construct a new HConverter that writes to a writer */
        HConverter(HWriter<Tout>* writer, size_t blocksize):
            _reader(nullptr),
            _writer(writer),
            _blocksize(blocksize),
            _input(NULL)
        {
            _output = new Tout[blocksize];
        }

        /** Construct a new HConverter that registers with an upstream writer */
        HConverter(HWriterConsumer<Tin>* consumer, size_t blocksize):
            _blocksize(blocksize),
            _input(NULL)
        {
            _output = new Tout[blocksize];
            consumer->SetWriter(this);
        }

    public:

        /** Default destructor */
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

        /** Read a block of converted samples */
        int Read(Tout* dest, size_t blocksize) {
            if( blocksize != _blocksize )
            {
                HError("Request for conversion-read with blocksize %d expected %d blocks", blocksize, _blocksize);
                throw new HConverterIOException("Invalid number of blocks");
            }

            int read;
            if( (read = _reader->Read(_input, blocksize)) != blocksize )
            {
                HError("Request for read with blocksize %d returned %d blocks", blocksize, read);
                throw new HConverterIOException("Invalid number of blocks");
            }

            int converted;
            if( (converted = Convert(_input, dest, blocksize)) != blocksize )
            {
                HError("Request for conversion of blocksize %d blocks converted %d blocks", blocksize, converted);
                throw new HConverterIOException("Converted incorrect number of blocks");
            }

            return blocksize;
        };

        /** Write a block of samples to convert */
        int Write(Tin* src, size_t blocksize) {
            if( blocksize != _blocksize )
            {
                HError("Request for conversion-write with blocksize %d expected %d blocks", blocksize, _blocksize);
                throw new HConverterIOException("Read incorrect number of blocks from input reader");
            }

            int converted;
            if( (converted = Convert(src, _output, blocksize)) != blocksize )
            {
                HError("Request for conversion of blocksize %d blocks converted %d blocks", blocksize, converted);
                throw new HConverterIOException("Converted incorrect number of blocks");
            }

            int written;
            if( (written = _writer->Write(_output, blocksize)) != blocksize )
            {
                HError("Request for write with blocksize %d wrote %d blocks", blocksize, written);
                throw new HConverterIOException("Wrote incorrect number of blocks to output writer");
            }

            return blocksize;
        }

        /** Convert a block of samples */
        virtual int Convert(Tin* src, Tout* dest, size_t blocksize) = 0;

        /** Implements HWriterConsumer::SetWriter() */
        void SetWriter(HWriter<Tout>* writer)
        {
            _writer = writer;
        }

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