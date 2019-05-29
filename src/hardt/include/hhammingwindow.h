#ifndef __HHAMMINGWINDOW_H
#define __HHAMMINGWINDOW_H

template <class T>
class HHammingWindow : public HWindow<T>
{
    private:

        float ValueAt(int N, int n);
};

#endif