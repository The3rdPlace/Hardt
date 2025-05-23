#ifndef __HDEMUX_H
#define __HDEMUX_H

/**
    De-multiplex a stream of samples.
    The incomming samples are written round-robin to the writer given in the
    list when constructing the object.

    Ex: Having sampled a signal from 2 channels, writing the stream of
    samples from soundcard to an instanse of this class initiaized with
    two writers - would result in the two writers receaving either the
    left or the right samples. Another use is to split an IQ stream into
    an I and Q branch.

    The number of samples per block written to the output writers will be
    half the number of incomming samples in the above example.
*/

template <class T>
class HDeMux : public HWriter<T>, public HWriterConsumer<T>, public HReader<T>
{
    private:

        std::vector< HWriter<T>* > _writers;
        HReader<T>* _reader;
        T** _buffers;
        size_t _blocksize;

        int _readers;
        T* _readBuffer;
        int _nextReader;

    public:

        /** Construct a new HDemux object that writes to a writer */
        HDeMux(std::string id, std::vector< HWriter<T>* > writers, size_t blocksize);

        /** Construct a new HDemux object that registers with an upstream writer */
        HDeMux(std::string id, HWriterConsumer<T>* consumer, size_t blocksize);

        /** Construct a new HDemux object that reads from a reader */
        HDeMux(std::string id, HReader<T>* reader, int readers, size_t blocksize);

        /** Default destructor */
        ~HDeMux();

        /** Write a block of samples */
        int WriteImpl(T* src, size_t blocksize);

        /** Read a block of samples */
        int ReadImpl(T* dest, size_t blocksize);

        /** Implements HWriterConsumer::SetWriter() */
        void SetWriter(HWriter<T>* writer)
        {
            if( _buffers != NULL )
            {
                for( int i = 0; i < _writers.size(); i++ )
                {
                    delete _buffers[i];
                }
                delete _buffers;
            }

            _writers.push_back(writer);

            _buffers = new T*[_writers.size()];
            for( int i = 0; i < _writers.size(); i++ )
            {
                _buffers[i] = new T[_blocksize / _writers.size()];
            }
        }

        /** Forward a command */
        bool Command(HCommand* command) {
            typename std::vector< HWriter<T>* >::iterator it;

            for( it = _writers.begin(); it != _writers.end(); it++ ) {
                if( !(*it)->Command(command) ) {
                    return false;
                }
            }
            return true;
        }

        /** Initialize before first read/write */
        bool Start() {
            typename std::vector< HWriter<T>* >::iterator it;

            for( it = _writers.begin(); it != _writers.end(); it++ ) {
                if( !(*it)->Start() ) {
                    return false;
                }
            }
            return true;
        }

        /** Cleanup after last read/write */
        bool Stop() {
            typename std::vector< HWriter<T>* >::iterator it;

            for( it = _writers.begin(); it != _writers.end(); it++ ) {
                if( !(*it)->Stop() ) {
                    return false;
                }
            }
            return true;
        }
};

#endif