#ifndef __HIQFIRFILTER_H
#define __HIQFIRFILTER_H

#include "hfir.h"

#include <fstream>

/**
    A generic FIR-type filter.

    When created, a list of coefficients must be provided.
    The coefficients is expected to be the normalized and
    scaled coefficients.

    Coefficients must be provided in the order a0, ..., aN
*/
template <class T>
class HIqFirFilter : public HFilter<T>
{
    private:

        HFir<T>* _firI;
        HFir<T>* _firQ;

        void Init(float* coefficients, int length, size_t blocksize);

    public:

        /** Construct a new HIqFirFilter that writes to a writer */
        HIqFirFilter(std::string id, HWriter<T>* writer, float* coefficients, int length, size_t blocksize);

        /** Construct a new HIqFirFilter that registers with an upstream writer */
        HIqFirFilter(std::string id, HWriterConsumer<T>* consumer, float* coefficients, int length, size_t blocksize);

        /** Construct a new HIqFirFilter that reads from a reader */
        HIqFirFilter(std::string id, HReader<T>* reader, float* coefficients, int length, size_t blocksize);

        /** Default destructor */
        ~HIqFirFilter();

        /** Run a block of samples through the filter */
        void Filter(T* src, T* dest, size_t blocksize);

        /** Set the filter coefficients */
        void SetCoefficients(float* coefficients, int length);

        /** Get the filter coefficients */
        std::vector<float> GetCoefficients()
        {
            return _firI->GetCoefficients();
        }

        /** Factory function to create a new HIqFirFilter that writes to a  writer */
        static HIqFirFilter<T>* Create(std::string id, HWriter<T>* writer, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = HFilter<T>::ReadCoeffsFromFile(coeffsFilename);

            return new HIqFirFilter<T>(id, writer, coeffs.data(), coeffs.size(), blocksize);
        }

        /** Factory function to create a new HIqFirFilter that registers with an upstream writer */
        static HIqFirFilter<T>* Create(std::string id, HWriterConsumer<T>* consumer, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = HFilter<T>::ReadCoeffsFromFile(coeffsFilename);

            return new HIqFirFilter<T>(id, consumer, coeffs.data(), coeffs.size(), blocksize);
        }

        /** Factory function to create a new HIqFirFilter that reads from a reader */
        static HIqFirFilter<T>* Create(std::string id, HReader<T>* reader, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = HFilter<T>::ReadCoeffsFromFile(coeffsFilename);

            return new HIqFirFilter<T>(id, reader, coeffs.data(), coeffs.size(), blocksize);
        }
};

#endif