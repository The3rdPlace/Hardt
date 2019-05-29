#ifndef __HRECTANGULARWINDOW_H
#define __HRECTANGULARWINDOW_H

template <class T>
class HRectangularWindow : public HWindow<T>
{
    private:

        float ValueAt(int N, int n);
};

#endif