#ifndef __HSIGNALLEVEL_H
#define __HSIGNALLEVEL_H

struct HSignalLevelResult
{
    int Max;
    int Db;
    int S;
};

template <class T>
class HSignalLevel : public HOutput<T, HSignalLevelResult>
{
    private:

        HSignalLevelResult _result;
        int _blocksReceived;
        int _skip;
        int _ref;

        float _factor;

    public:

        HSignalLevel(HWriter<HSignalLevelResult>* writer, int skip, int ref = 54);

        int Output(T* src, size_t blocksize);
};

#endif
