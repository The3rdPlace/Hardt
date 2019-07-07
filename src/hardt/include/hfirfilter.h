#ifndef __HFIRFILTER_H
#define __HFIRFILTER_H

#include <fstream>

template <class T>
class HFirFilter : public HFilter<T>
{
    private:

        float* _coefficients;
        int _length;
        T* _taps;

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

        HFirFilter(HWriter<T>* writer, float* coefficients, int length, size_t blocksize);
        HFirFilter(HReader<T>* reader, float* coefficients, int length, size_t blocksize);

        ~HFirFilter();

        void Filter(T* src, T* dest, size_t blocksize);

        static HFirFilter<T>* Create(HWriter<T>* writer, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = ReadCoeffsFromFile(coeffsFilename);

            return new HFirFilter<T>(writer, coeffs.data(), coeffs.size(), blocksize);
        }

        static HFirFilter<T>* Create(HReader<T>* reader, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = ReadCoeffsFromFile(coeffsFilename);

            return new HFirFilter<T>(reader, coeffs.data(), coeffs.size(), blocksize);
        }
};

#endif