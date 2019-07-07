#ifndef __HIIRFILTER_H
#define __HIIRFILTER_H

#include <fstream>

template <class T>
class HIirFilter : public HFilter<T>
{
    private:

        float* _aCoefficients;
        float* _bCoefficients;
        int _length;
        T* _taps;
        float* _output;

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

        void Init(float* coefficients, int length);

    public:

        HIirFilter(HWriter<T>* writer, float* coefficients, int length, size_t blocksize);
        HIirFilter(HReader<T>* reader, float* coefficients, int length, size_t blocksize);

        ~HIirFilter();

        void Filter(T* src, T* dest, size_t blocksize);

        static HIirFilter<T>* Create(HWriter<T>* writer, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = ReadCoeffsFromFile(coeffsFilename);

            return new HIirFilter<T>(writer, coeffs.data(), coeffs.size(), blocksize);
        }

        static HIirFilter<T>* Create(HReader<T>* reader, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = ReadCoeffsFromFile(coeffsFilename);

            return new HIirFilter<T>(reader, coeffs.data(), coeffs.size(), blocksize);
        }
};

#endif