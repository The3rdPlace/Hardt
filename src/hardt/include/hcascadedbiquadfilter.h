#ifndef __HCASCADEDBIQUADFACTORY_H
#define __HCASCADEDBIQUADFACTORY_H

/**
    A generic cascaded biquad filter (a number of biquads
    cascaded efter each other).

    Given a set of coefficients, usually calculated by a
    filterdesign tool - but they can be taken from one or
    several generic biquads (HHighpassBiQuad, HNotchBiQuad, etc.).

    The parameters must be provided in the order:
    b0,1  b1,1  b2,1  a1,1  a2,1
    b0,2  b1,2  b2,2  a1,2  a2,2
    b0,...  b1,...  b2,...  a1,...  a2,...
    b0,N  b1,N  b2,N  a1,N  a2,N

    The coefficients can either be defined as an array in the source code, or
    come from a file - use the factory method Create() to read such coefficients.
    (this is practical when you are finetuning a filter)
*/
template <class T>
class HCascadedBiQuadFilter: public HReader<T>, public HWriter<T>
{
    private:

        HWriter<T>* _writer;
        HReader<T>* _reader;

        int _blocksize;

        HIirFilter<T>** _filters;
        int _filterCount;
        int _firstLength;

        void Init(float* coefficients, int length);

    public:

        HCascadedBiQuadFilter(HWriter<T>* writer, float* coefficients, int length, size_t blocksize);
        HCascadedBiQuadFilter(HReader<T>* reader, float* coefficients, int length, size_t blocksize);

        ~HCascadedBiQuadFilter();

        int Write(T* src, size_t blocksize);
        int Read(T* dest, size_t blocksize);

        bool Start();
        bool Stop();

        void SetCoefficients(float* coefficients, int length);

        static HCascadedBiQuadFilter<T>* Create(HWriter<T>* writer, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = HFilter<T>::ReadCoeffsFromFile(coeffsFilename);

            return new HCascadedBiQuadFilter<T>(writer, coeffs.data(), coeffs.size(), blocksize);
        }

        static HCascadedBiQuadFilter<T>* Create(HReader<T>* reader, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = HFilter<T>::ReadCoeffsFromFile(coeffsFilename);

            return new HCascadedBiQuadFilter<T>(reader, coeffs.data(), coeffs.size(), blocksize);
        }
};

#endif