#ifndef __HARDTAPI_H
#define __HARDTAPI_H

template <class T>
class HFileWriter
{
    public:
    HFileWriter(const char* path);
};

template <>
extern HFileWriter<int>::HFileWriter(const char* path);

#endif
