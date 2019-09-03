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

        int _blocksize;
        T* _buffer;

        void Init();

    protected:

        /** Construct a new HFilter that writes to a writer */
        HFilter(HWriter<T>* writer, size_t blocksize);

        /** Construct a new HFilter that registers with an upstream writer */
        HFilter(HWriterConsumer<T>* consumer, size_t blocksize);

        /** Construct a new HFilter that reads from a reader */
        HFilter(HReader<T>* reader, size_t blocksize);

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

    public:

        /** Utility function that reads a list of floating point coefficients from a file */
        static std::vector<float> ReadCoeffsFromFile(char* filename)
        {
            std::vector<float> coeffs;

            std::ifstream coeffsFile(filename);
            if (coeffsFile.is_open())
            {
                try
                {
                    float a;
                    while (coeffsFile >> a)
                    {
                        coeffs.push_back(a);
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