#ifndef __HCASCADEDBIQUADFACTORY_H
#define __HCASCADEDBIQUADFACTORY_H

template <class T>
class HCascadedBiQuadFilter: public HReader<T>, public HWriter<T>
{
    private:

        HWriter<T>* _writer;
        HReader<T>* _reader;

        int _blocksize;

        HIirFilter<T>** _filters;
        int _filterCount;

        void Init(float* coefficients, int length);

    public:

        HCascadedBiQuadFilter(HWriter<T>* writer, float* coefficients, int length, size_t blocksize);
        HCascadedBiQuadFilter(HReader<T>* reader, float* coefficients, int length, size_t blocksize);

        ~HCascadedBiQuadFilter();

        int Write(T* src, size_t blocksize);
        int Read(T* dest, size_t blocksize);

        bool Start();
        bool Stop();

};

#endif