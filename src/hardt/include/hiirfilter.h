#ifndef __HIIRFILTER_H
#define __HIIRFILTER_H

#include <fstream>

/**
    Generic IIR filter. The filter is implemented in the first direct form.

    The coefficients for the filter must be given in the floating point
    array, and the coefficients must be ordered this way:

    b0, b1, b2, b..., bN, a1, a2, a..., aN
*/

template <class T>
class HIirFilter : public HFilter<T>
{
    private:

        float* _aCoefficients;
        float* _bCoefficients;
        int _length;
        int _firstLength;
        T* _taps;
        float* _output;

    protected:

        void Init(float* coefficients, int length);

    public:

        HIirFilter(HWriter<T>* writer, float* coefficients, int length, size_t blocksize);
        HIirFilter(HWriterConsumer<T>* consumer, float* coefficients, int length, size_t blocksize);
        HIirFilter(HReader<T>* reader, float* coefficients, int length, size_t blocksize);

        ~HIirFilter();

        virtual void Filter(T* src, T* dest, size_t blocksize);

        void SetCoefficients(float* coefficients, int length);

        std::vector<float> GetCoefficients()
        {
            std::vector<float> coefficients;
            for( int i = 0; i <= _length; i++ )
            {
                coefficients.push_back(_bCoefficients[i]);
            }
            for( int i = 0; i < _length; i++ )
            {
                coefficients.push_back(_aCoefficients[i]);
            }
            return coefficients;
        }

        static HIirFilter<T>* Create(HWriter<T>* writer, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = HFilter<T>::ReadCoeffsFromFile(coeffsFilename);

            return new HIirFilter<T>(writer, coeffs.data(), coeffs.size(), blocksize);
        }

        static HIirFilter<T>* Create(HReader<T>* reader, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = HFilter<T>::ReadCoeffsFromFile(coeffsFilename);

            return new HIirFilter<T>(reader, coeffs.data(), coeffs.size(), blocksize);
        }
};

#endif