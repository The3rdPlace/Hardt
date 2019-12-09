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

        /** Initialize the filter */
        void Init(float* coefficients, int length);

    public:

        /** Construct a new HIIrFilter that writes to a writer */
        HIirFilter(HWriter<T>* writer, float* coefficients, int length, size_t blocksize, HProbe<T>* prope = NULL);

        /** Construct a new HIIrFilter that registers with an upstream writer */
        HIirFilter(HWriterConsumer<T>* consumer, float* coefficients, int length, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HIirFilter that reads from a reader */
        HIirFilter(HReader<T>* reader, float* coefficients, int length, size_t blocksize, HProbe<T>* probe = NULL);

        /** Default destructor */
        ~HIirFilter();

        /** Run a block of samples through the filter */
        virtual void Filter(T* src, T* dest, size_t blocksize);

        /** Set the coefficients of the filter. The coefficients must be provided in the order:
            b0, b1, b2, ..., bN, a1, a2, ..., aN */
        void SetCoefficients(float* coefficients, int length);

        /** Get the filter coefficients. The coefficients is returned in the order:
            b0, b1, b2, ..., bN, a1, a2, ..., aN */
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

        /** Factory function: Create a new HIirFilter, that writes to a writer, by reading the coefficients from a file */
        static HIirFilter<T>* Create(HWriter<T>* writer, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = HFilter<T>::ReadCoeffsFromFile(coeffsFilename);

            return new HIirFilter<T>(writer, coeffs.data(), coeffs.size(), blocksize);
        }

        /** Factory function: Create a new HIirFilter, that registers with an upstream writer, by reading the coefficients from a file */
        static HIirFilter<T>* Create(HWriterConsumer<T>* consumer, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = HFilter<T>::ReadCoeffsFromFile(coeffsFilename);

            return new HIirFilter<T>(consumer, coeffs.data(), coeffs.size(), blocksize);
        }

        /** Factory function: Create a new HIirFilter that reads from a reader, by reading the coefficients from a file */
        static HIirFilter<T>* Create(HReader<T>* reader, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = HFilter<T>::ReadCoeffsFromFile(coeffsFilename);

            return new HIirFilter<T>(reader, coeffs.data(), coeffs.size(), blocksize);
        }
};

#endif