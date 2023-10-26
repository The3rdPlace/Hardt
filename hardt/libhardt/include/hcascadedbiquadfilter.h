#ifndef __HCASCADEDBIQUADFILTER_H
#define __HCASCADEDBIQUADFILTER_H

#include "hwriter.h"
#include "hwriterconsumer.h"
#include "hprobe.h"
#include "hreader.h"

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
class HCascadedBiQuadFilter: public HReader<T>, public HWriter<T>, public HWriterConsumer<T>
{
    private:

        bool _isReader;
        bool _isWriter;

        int _blocksize;

        HIirFilter<T>** _filters;
        int _filterCount;
        int _firstLength;

        void Init(int length);

        void SetWriter(HWriter<T>* writer);

    public:

        /** Construct a new HCascadedBiQuadFilter */
        HCascadedBiQuadFilter(std::string id, HWriter<T>* writer, float* coefficients, int length, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HCascadedBiQuadFilter */
        HCascadedBiQuadFilter(std::string id, HWriterConsumer<T>* consumer, float* coefficients, int length, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HCascadedBiQuadFilter */
        HCascadedBiQuadFilter(std::string id, HReader<T>* reader, float* coefficients, int length, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HCascadedBiQuadFilter */
        HCascadedBiQuadFilter(std::string id, HWriter<T>* writer, std::vector<float*> biquadCoefficients, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HCascadedBiQuadFilter */
        HCascadedBiQuadFilter(std::string id, HWriterConsumer<T>* consumer, std::vector<float*> biquadCoefficients, size_t blocksize, HProbe<T>* probe = NULL);

        /** Construct a new HCascadedBiQuadFilter */
        HCascadedBiQuadFilter(std::string id, HReader<T>* reader, std::vector<float*> biquadCoefficients, size_t blocksize, HProbe<T>* probe = NULL);

        /** Default destructor */
        ~HCascadedBiQuadFilter();

        /** Write a block of samples to the filter */
        int Write(T* src, size_t blocksize);

        /** Read a block of samples from the filter */
        int Read(T* dest, size_t blocksize);

        /** Prepare filter for operation */
        bool Start();

        /** Shutdown the filter */
        bool Stop();

        /** Set filter coefficients */
        void SetCoefficients(float* coefficients, int length);

        /** Factory method to create a new cascaded filter from the name of a file with coefficients */
        static HCascadedBiQuadFilter<T>* Create(std::string id, HWriter<T>* writer, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = HFilter<T>::ReadCoeffsFromFile(coeffsFilename);

            return new HCascadedBiQuadFilter<T>(id, writer, coeffs.data(), coeffs.size(), blocksize);
        }

        /** Factory method to create a new cascaded filter from the name of a file with coefficients */
        static HCascadedBiQuadFilter<T>* Create(std::string id, HWriterConsumer<T>* consumer, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = HFilter<T>::ReadCoeffsFromFile(coeffsFilename);

            return new HCascadedBiQuadFilter<T>(id, consumer, coeffs.data(), coeffs.size(), blocksize);
        }

        /** Factory method to create a new cascaded filter from the name of a file with coefficients */
        static HCascadedBiQuadFilter<T>* Create(std::string id, HReader<T>* reader, size_t blocksize, char* coeffsFilename)
        {
            std::vector<float> coeffs = HFilter<T>::ReadCoeffsFromFile(coeffsFilename);

            return new HCascadedBiQuadFilter<T>(id, reader, coeffs.data(), coeffs.size(), blocksize);
        }

        /** Execute or carry through a command */
        bool Command(HCommand* command) {
            if( _filterCount > 0 ) {
                return _filters[0]->Command(command);
            }
            return true;
        }
};

#endif