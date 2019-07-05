#ifndef __HFIRFILTER_H
#define __HFIRFILTER_H

template <class T>
class HFirFilter : public HFilter<T>
{
    private:

        float* _coefficients;
        int _length;
        T* _taps;

    public:

        HFirFilter(HWriter<T>* writer, float* coefficients, int length, size_t blocksize);
        HFirFilter(HReader<T>* reader, float* coefficients, int length, size_t blocksize);

        ~HFirFilter();

        void Filter(T* src, T* dest, size_t blocksize);
};

#endif