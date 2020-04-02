#ifndef __HFILTER_H
#define __HFILTER_H

#include "hprobe.h"

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

        HProbe<T>* _probe;

        int _blocksize;
        T* _buffer;

        void Init();

    protected:

        /** Construct a new HFilter that writes to a writer */
        HFilter(HWriter<T>* writer, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HFilter that registers with an upstream writer */
        HFilter(HWriterConsumer<T>* consumer, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HFilter that reads from a reader */
        HFilter(HReader<T>* reader, size_t blocksize, HProbe<T>* probe = NULL);

        /** Implements HWriterConsumer::SetWriter() */
        void SetWriter(HWriter<T>* writer)
        {
            _writer = writer;
        }

    public:

        /** Default destructor */
        ~HFilter();

        /** Write a block of samples */
        int Write(T* src, size_t blocksize);

        /** Read a block of samples */
        int Read(T* dest, size_t blocksize);

        /** Run a block of samples through the filter */
        virtual void Filter(T* src, T* dest, size_t blocksize) = 0;

        /** Initialize before first read/write */
        bool Start();

        /** Cleanup after last read/write */
        bool Stop();

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            if( _writer != nullptr ) {
                return _writer->Command(command);
            } else if( _reader != nullptr ) {
                return _reader->Command(command);
            }
            return true;
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