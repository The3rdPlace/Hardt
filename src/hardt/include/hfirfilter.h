#ifndef __HFIRFILTER_H
#define __HFIRFILTER_H

#include <fstream>

/**
    A generic FIR-type filter.

    When created, a list of coefficients must be provided.
    The coefficients is expected to be the normalized and
    scaled coefficients.

    Coefficients must be provided in the order a0, ..., aN
*/
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

        /** Construct a new HFirFilter that writes to a writer */
        HFirFilter(HWriter<T>* writer, float* coefficients, int length, size_t blocksize);

        /** Construct a new HFirFilter that registers with an upstream writer */
        HFirFilter(HWriterConsumer<T>* consumer, float* coefficients, int length, size_t blocksize);

        /** Construct a new HFirFilter that reads from a reader */
        HFirFilter(HReader<T>* reader, float* coefficients, int length, size_t blocksize);

        /** Default destructor */
        ~HFirFilter();

        /** Run a block of samples through the filter */
        void Filter(T* src, T* dest, size_t blocksize);

        /** Set the filter coefficients */
        void SetCoefficients(float* coefficients, int length);

        /** Get the filter coefficients */
        std::vector<float> GetCoefficients()
        {
            std::vector<float> coefficients;
            for( int i = 0; i < _length; i++ )
            {
                coefficients.push_back(_coefficients[i]);
            }
            return coefficients;
        }

        /** Factory function to create a new HFirFilter that writes to a  writer */
        static HFirFilter<T>* Create(HWriter<T>* writer, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = HFilter<T>::ReadCoeffsFromFile(coeffsFilename);

            return new HFirFilter<T>(writer, coeffs.data(), coeffs.size(), blocksize);
        }

        /** Factory function to create a new HFirFilter that registers with an upstream writer */
        static HFirFilter<T>* Create(HWriterConsumer<T>* consumer, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = HFilter<T>::ReadCoeffsFromFile(coeffsFilename);

            return new HFirFilter<T>(consumer, coeffs.data(), coeffs.size(), blocksize);
        }

        /** Factory function to create a new HFirFilter that reads from a reader */
        static HFirFilter<T>* Create(HReader<T>* reader, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = HFilter<T>::ReadCoeffsFromFile(coeffsFilename);

            return new HFirFilter<T>(reader, coeffs.data(), coeffs.size(), blocksize);
        }
};

#endif