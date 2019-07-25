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
        int _firstLength;

        void Init(float* coefficients, int length);

    public:

        HFirFilter(HWriter<T>* writer, float* coefficients, int length, size_t blocksize);
        HFirFilter(HReader<T>* reader, float* coefficients, int length, size_t blocksize);

        ~HFirFilter();

        void Filter(T* src, T* dest, size_t blocksize);

        void SetCoefficients(float* coefficients, int length);

        std::vector<float> GetCoefficients()
        {
            std::vector<float> coefficients;
            for( int i = 0; i < _length; i++ )
            {
                coefficients.push_back(_coefficients[i]);
            }
            return coefficients;
        }

        static HFirFilter<T>* Create(HWriter<T>* writer, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = HFilter<T>::ReadCoeffsFromFile(coeffsFilename);

            return new HFirFilter<T>(writer, coeffs.data(), coeffs.size(), blocksize);
        }

        static HFirFilter<T>* Create(HReader<T>* reader, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = HFilter<T>::ReadCoeffsFromFile(coeffsFilename);

            return new HFirFilter<T>(reader, coeffs.data(), coeffs.size(), blocksize);
        }
};

#endif