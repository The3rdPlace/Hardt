#ifndef __HFILTER_H
#define __HFILTER_H

/**
    Base class for filter implementations.
    This class defines a pure virtual method, Filter(), which must be
    implemented by classes inheriting this class.

    The constructor is protected, this class cannot be instantianted directly.
*/
template <class T>
class HFilter : public HFilterBase<T>, public HWriter<T>, public HReader<T>, public HWriterConsumer<T>
{
    private:

        HWriter<T>* _writer;
        HReader<T>* _reader;

        T* _buffer;

        bool _enabled;

        void Init() {

            _buffer = new T[_blocksize];
            HLog("Allocated %d as local buffer", _blocksize * sizeof(T));
        }

    protected:

        /** The filters set blocksize */
        int _blocksize;

        /** Construct a new HFilter that writes to a writer */
        HFilter(std::string id, HWriter<T>* writer, size_t blocksize):
            HReader<T>(id),
            HWriter<T>(id),
            HWriterConsumer<T>(id),
            _writer(writer),
            _reader(NULL),
            _blocksize(blocksize),
            _enabled(true) {
            HLog("HFilter(HWriter*)");
            Init();
        }

        /** Construct a new HFilter that registers with an upstream writer */
        HFilter(std::string id, HWriterConsumer<T>* consumer, size_t blocksize):
            HReader<T>(id),
            HWriter<T>(id),
            HWriterConsumer<T>(id),
            _reader(NULL),
            _blocksize(blocksize),
            _enabled(true) {
            HLog("HFilter(HWriter*)");
            Init();

            consumer->SetWriter(this);
        }

        /** Construct a new HFilter that reads from a reader */
        HFilter(std::string id, HReader<T>* reader, size_t blocksize):
            HReader<T>(id),
            HWriter<T>(id),
            HWriterConsumer<T>(id),
            _writer(NULL),
            _reader(reader),
            _blocksize(blocksize),
            _enabled(true) {
            HLog("HFilter(HReader*)");
            Init();
        }

        /** Implements HWriterConsumer::SetWriter() */
        void SetWriter(HWriter<T>* writer)
        {
            _writer = writer;
        }

    public:

        /** Default destructor */
        ~HFilter() {
            HLog("~HFilter()");
            delete _buffer;
        }

        /** Write a block of samples */
        int WriteImpl(T* src, size_t blocksize) {

            if( blocksize > _blocksize )
            {
                HError("Illegal blocksize in Write() to HFilter. Initialized with %d called with %d", _blocksize, blocksize);
                throw new HFilterIOException("It is not allowed to write more data than the size given at creation of the filter");
            }

            if( _enabled ) {
                Filter(src, _buffer, blocksize);
            } else {
                memcpy((void*) _buffer, (void*) src, sizeof(T) * blocksize);
            }
            int written = _writer->Write(_buffer, blocksize);

            return written;
        }

        /** Read a block of samples */
        int ReadImpl(T* dest, size_t blocksize) {

            if( blocksize > _blocksize )
            {
                HError("Illegal blocksize in Read() to HFilter. Initialized with %d called with %d", _blocksize, blocksize);
                throw new HFilterIOException("It is not possible to read more data than the size given at creation of the filter");
            }

            int received = _reader->Read(_buffer, blocksize);
            if( _enabled ) {
                Filter(_buffer, dest, received);
            } else {
                memcpy((void*) dest, (void*) _buffer, sizeof(T) * blocksize);
            }

            return received;
        }

        /** Run a block of samples through the filter */
        virtual void Filter(T* src, T* dest, size_t blocksize) = 0;

        /** Initialize before first read/write */
        bool Start() {

            if( _reader != NULL )
            {
                HLog("Calling Start() on reader");
                return _reader->Start();
            }
            if( _writer != NULL )
            {
                HLog("Calling Start() on writer");
                return _writer->Start();
            }
            return false;
        }

        /** Cleanup after last read/write */
        bool Stop() {

            if( _reader != NULL )
            {
                HLog("Calling Stop() on reader");
                return _reader->Stop();
            }
            if( _writer != NULL )
            {
                HLog("Calling Stop() on writer");
                return _writer->Stop();
            }
            return false;
        }

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            if( _writer != nullptr ) {
                return _writer->Command(command);
            } else if( _reader != nullptr ) {
                return _reader->Command(command);
            }
            return true;
        }

        /** Enable the filter (the filter is always enabled after construction) */
        void Enable() {
            _enabled = true;
        }

        /** Disable the filter */
        void Disable() {
            _enabled = false;
        }

        /** Returns true if the filter is enabled (filter is always enabled after construction), false otherwise */
        bool GetEnabled() {
            return _enabled;
        }

    public:

        /** Utility function that reads a list of floating point coefficients from a file

            It can handle files with one parameter on each line or several parameters on each line
            separated by comma, blankspace or semicolon. It will ignore other junk (characters) and
            terminates a line on a c-style single line comment ('//') */
        static std::vector<float> ReadCoeffsFromFile(std::string filename)
        {
            char name[filename.length() + 1];
            strcpy(name, filename.c_str());

            return ReadCoeffsFromFile(name);
        }

        /** Utility function that reads a list of floating point coefficients from a file

            It can handle files with one parameter on each line or several parameters on each line
            separated by comma, blankspace or semicolon. It will ignore other junk (characters) and
            terminates a line on a c-style single line comment ('//') */
        static std::vector<float> ReadCoeffsFromFile(char* filename)
        {
            std::vector<float> coeffs;

            #define BEGIN 0
            #define DIGIT 1
            #define SEPARATOR 2
            #define JUNK 3
            #define COMMENT_BEGIN 4
            #define COMMENT 5

            std::ifstream coeffsFile(filename);
            if (coeffsFile.is_open())
            {
                try
                {
                    std::string block;

                    while( !coeffsFile.eof() )
                    {
                        std::getline(coeffsFile, block);

                        int state = BEGIN;
                        std::string washed = "";
                        for( int i = 0; i < block.length(); i++ )
                        {
                            switch(state)
                            {
                                case BEGIN:
                                    switch(block[i])
                                    {
                                        case '0':
                                        case '1':
                                        case '2':
                                        case '3':
                                        case '4':
                                        case '5':
                                        case '6':
                                        case '7':
                                        case '8':
                                        case '9':
                                        case '-':
                                        case '+':
                                            state = DIGIT;
                                            washed += block[i];
                                            break;
                                        case ' ':
                                        case ',':
                                        case ';':
                                            state = SEPARATOR;
                                            break;
                                        case '/':
                                            state = COMMENT_BEGIN;
                                            break;
                                        default:
                                            state = JUNK;
                                            break;
                                    }
                                    break;

                                case DIGIT:
                                    switch(block[i]) {
                                        case '0':
                                        case '1':
                                        case '2':
                                        case '3':
                                        case '4':
                                        case '5':
                                        case '6':
                                        case '7':
                                        case '8':
                                        case '9':
                                        case '.':
                                            state = DIGIT;
                                            washed += block[i];
                                            break;
                                        case ' ':
                                        case ',':
                                        case ';':
                                            if( washed.length() > 0 ) {
                                                coeffs.push_back(std::stof(washed));
                                                washed = "";
                                            }
                                            state = SEPARATOR;
                                            break;
                                        case '/':
                                            if( washed.length() > 0 ) {
                                                coeffs.push_back(std::stof(washed));
                                                washed = "";
                                            }
                                            state = COMMENT_BEGIN;
                                            break;
                                        default:
                                            if( washed.length() > 0 ) {
                                                coeffs.push_back(std::stof(washed));
                                                washed = "";
                                            }
                                            state = JUNK;
                                            break;
                                    }
                                    break;

                                case SEPARATOR:
                                    switch(block[i]) {
                                        case '0':
                                        case '1':
                                        case '2':
                                        case '3':
                                        case '4':
                                        case '5':
                                        case '6':
                                        case '7':
                                        case '8':
                                        case '9':
                                        case '-':
                                        case '+':
                                            state = DIGIT;
                                            washed += block[i];
                                            break;
                                        case ' ':
                                        case ',':
                                        case ';':
                                            state = SEPARATOR;
                                            break;
                                        default:
                                            state = JUNK;
                                            break;
                                    }
                                    break;

                                case JUNK:
                                    switch(block[i])
                                    {
                                        case ' ':
                                        case ',':
                                        case ';':
                                            state = SEPARATOR;
                                            break;
                                        default:
                                            state = JUNK;
                                            break;
                                    }

                                case COMMENT_BEGIN:
                                    switch(block[i]) {
                                        case '/':
                                            state = COMMENT;
                                            break;
                                        default:
                                            state = JUNK;
                                            break;
                                    }
                                    break;

                                case COMMENT:
                                    state = COMMENT;
                            }
                        }

                        // Add remaining digits, if we are still reading digits
                        if( state == DIGIT && washed.length() > 0 )
                        {
                            coeffs.push_back(std::stof(washed));
                        }
                    }
                }
                catch( std::exception* ex )
                {
                    HError("Caught exception while reading coefficients file: %s", ex->what());
                    throw new HFilterInitializationException("Exception while reading coefficients from file");
                }
                catch( ... )
                {
                    HError("Caught unknown exception while reading coefficients file");
                    throw new HFilterInitializationException("Exception while reading coefficients from file");
                }
            }
            else
            {
                HError("No such file %s", filename);
                throw new HFilterInitializationException("Coefficients file not found");
            }
            coeffsFile.close();

            return coeffs;
        }

};

#endif