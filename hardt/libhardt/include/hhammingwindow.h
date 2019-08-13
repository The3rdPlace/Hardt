#ifndef __HHAMMINGWINDOW_H
#define __HHAMMINGWINDOW_H

/**
    A Hamming window
*/
template <class T>
class HHammingWindow : public HWindow<T>
{
    private:

        float ValueAt(int N, int n);
};

#endif