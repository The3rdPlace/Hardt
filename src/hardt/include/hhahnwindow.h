#ifndef __HHAHNWINDOW_H
#define __HHAHNWINDOW_H

template <class T>
class HHahnWindow : public HWindow<T>
{
    private:

        float ValueAt(int N, int n);
};

#endif