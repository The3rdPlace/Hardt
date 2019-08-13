#ifndef __HHAHNWINDOW_H
#define __HHAHNWINDOW_H

/**
    A Hahn window
*/
template <class T>
class HHahnWindow : public HWindow<T>
{
    private:

        float ValueAt(int N, int n);
};

#endif