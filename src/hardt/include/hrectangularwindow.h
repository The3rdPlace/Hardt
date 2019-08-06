#ifndef __HRECTANGULARWINDOW_H
#define __HRECTANGULARWINDOW_H

/**
    A rectangular window.
    It passes samples through multiplied by a constant 1 (one)
    so in effect, the signal just passes through unmodified.
*/
template <class T>
class HRectangularWindow : public HWindow<T>
{
    private:

        float ValueAt(int N, int n);
};

#endif