#ifndef __HWINDOW_H
#define __HWINDOW_H

/**
    Base class for windows
*/
template <class T>
class HWindow
{
    protected:

        /** Contains the precalculated window values */
        float* _valueAt;

        /** The size of the window */
        int _N;

        /** Get the window value at this sample number */
        virtual float ValueAt(int N, int n) = 0;

    public:

        /** Construct a new HWindow */
        HWindow();

        /** Default destructor */
        ~HWindow();

        /** Set size of the window */
        void SetSize(int N);

        /** Apply the window to the given block of samples */
        void Apply(T* src, T* dest, size_t blocksize);
};

#endif