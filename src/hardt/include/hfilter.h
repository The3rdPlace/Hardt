#ifndef __HFILTER_H
#define __HFILTER_H

/**
    Base class for filter implementations.
    This class defines a pure virtual method, Filter(), which must be
    implemented by classes inheriting this class.

    The constructur is protected, this class cannot be instantianted directly.
*/
template <class T>
class HFilter : public HFilterBase<T>, public HWriter<T>, public HReader<T>//, HWriterConsumer<T>
{
    private:

        HWriter<T>* _writer;
        HReader<T>* _reader;

        int _blocksize;
        T* _buffer;

    protected:

        HFilter(HWriter<T>* writer, size_t blocksize);
        HFilter(HReader<T>* reader, size_t blocksize);

    public:

        ~HFilter();

        int Write(T* src, size_t blocksize);
        int Read(T* dest, size_t blocksize);

        virtual void Filter(T* src, T* dest, size_t blocksize) = 0;

        bool Start();
        bool Stop();

    public:

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