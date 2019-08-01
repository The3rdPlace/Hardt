#ifndef __HFILTERBASE_H
#define __HFILTERBASE_H

/**
    Defines the absolute base for any filter implementation.
    Contains only a pure virtual method Filter() which must
    be implemented.

    This class is most usefull when you need to provide a
    pointer to a generic filter to a method which ensures
    that the Filter() method is implemented, but has no
    other public methods available.

    If you whish to build a new type of filter, you
    should inherit HFilter instead of this class.
*/
template <class T>
class HFilterBase
{
    public:

        virtual void Filter(T* src, T* dest, size_t blocksize) = 0;
};

#endif