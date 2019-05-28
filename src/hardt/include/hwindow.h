#ifndef __HWINDOW_H
#define __HWINDOW_H

template <class T>
class HWindow
{
    protected:

        float* _valueAt;
        int _N;

        virtual float ValueAt(int N, int n) = 0;

    public:

        HWindow();
        ~HWindow();

        void SetSize(int N);
        void Apply(T* src, T* dest, size_t blocksize);
};

#endif