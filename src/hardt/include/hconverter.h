#ifndef __HCONVERTER_H
#define __HCONVERTER_H

// Note: This class is implemented fully in this header to allow any
//       Tin <==> Tout conversion to be defined without the need for
//       a huge number of explicisit instantiations

template <class Tin, class Tout>
class HConverter : public HReader<Tout>, HWriter<Tin>
{
    private:

        HReader<Tin>* _reader;
        HWriter<Tout>* _writer;
        size_t _blocksize;

        Tin *_input;
        Tout *_output;

    public:

        bool Start()
        {
            return (_reader != NULL ? _reader->Start() : true) && (_writer != NULL ? _writer->Start() : true);
        }

        bool Stop()
        {
            return (_reader != NULL ?_reader->Stop() : true) && (_writer != NULL ?_writer->Stop() : true);
        }

    public:

        HConverter(HReader<Tin>* reader, size_t blocksize):
            _reader(reader),
            _blocksize(blocksize),
            _output(NULL)
        {
            _input = new Tin[blocksize];
        }

        HConverter(HWriter<Tout>* writer, size_t blocksize):
            _writer(writer),
            _blocksize(blocksize),
            _input(NULL)
        {
            _output = new Tout[blocksize];
        }

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

        virtual int Convert(Tin* src, Tout* dest, size_t blocksize) = 0;
};

#endif