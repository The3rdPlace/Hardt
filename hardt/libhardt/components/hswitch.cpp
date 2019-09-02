#ifndef __HSWITCH_CPP
#define __HSWITCH_CPP

#include "hswitch.h"

template <class T>
HSwitch<T>::HSwitch(HWriter<T>* writer):
    _reader(NULL),
    _writer(writer)
{

}

template <class T>
HSwitch<T>::HSwitch(HWriterConsumer<T>* consumer):
    _reader(NULL),
    _writer(NULL)
{}

template <class T>
HSwitch<T>::HSwitch(HReader<T>* reader):
    _reader(reader),
    _writer(NULL)
{}

template <class T>
HSwitch<T>::~HSwitch()
{}

template <class T>
int HSwitch<T>::Write(T* src, size_t blocksize)
{
    return 0;
}

template <class T>
int HSwitch<T>::Read(T* dest, size_t blocksize)
{
    return 0;
}

template <class T>
void HSwitch<T>::SetPosition(int position)
{}

template <class T>
int HSwitch<T>::GetPosition()
{
    return _position;
}

template <class T>
void HSwitch<T>::Add(HReader<T>* reader)
{}

template <class T>
void HSwitch<T>::Add(HWriter<T>* writer)
{}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HSwitch()
template
HSwitch<int8_t>::HSwitch(HWriter<int8_t>* writer);

template
HSwitch<uint8_t>::HSwitch(HWriter<uint8_t>* writer);

template
HSwitch<int16_t>::HSwitch(HWriter<int16_t>* writer);

template
HSwitch<int32_t>::HSwitch(HWriter<int32_t>* writer);

template
HSwitch<int8_t>::HSwitch(HWriterConsumer<int8_t>* consumer);

template
HSwitch<uint8_t>::HSwitch(HWriterConsumer<uint8_t>* consumer);

template
HSwitch<int16_t>::HSwitch(HWriterConsumer<int16_t>* consumer);

template
HSwitch<int32_t>::HSwitch(HWriterConsumer<int32_t>* consumer);

template
HSwitch<int8_t>::HSwitch(HReader<int8_t>* reader);

template
HSwitch<uint8_t>::HSwitch(HReader<uint8_t>* reader);

template
HSwitch<int16_t>::HSwitch(HReader<int16_t>* reader);

template
HSwitch<int32_t>::HSwitch(HReader<int32_t>* reader);

// ~HSwitch()
template
HSwitch<int8_t>::~HSwitch();

template
HSwitch<uint8_t>::~HSwitch();

template
HSwitch<int16_t>::~HSwitch();

template
HSwitch<int32_t>::~HSwitch();

// Write()
template
int HSwitch<int8_t>::Write(int8_t* src, size_t blocksize);

template
int HSwitch<uint8_t>::Write(uint8_t* src, size_t blocksize);

template
int HSwitch<int16_t>::Write(int16_t* src, size_t blocksize);

template
int HSwitch<int32_t>::Write(int32_t* src, size_t blocksize);

// Read()
template
int HSwitch<int8_t>::Read(int8_t* dest, size_t blocksize);

template
int HSwitch<uint8_t>::Read(uint8_t* dest, size_t blocksize);

template
int HSwitch<int16_t>::Read(int16_t* dest, size_t blocksize);

template
int HSwitch<int32_t>::Read(int32_t* dest, size_t blocksize);

// SetPosition()
template
void HSwitch<int8_t>::SetPosition(int position);

template
void HSwitch<uint8_t>::SetPosition(int position);

template
void HSwitch<int16_t>::SetPosition(int position);

template
void HSwitch<int32_t>::SetPosition(int position);

// GetPosition()
template
int HSwitch<int8_t>::GetPosition();

template
int HSwitch<uint8_t>::GetPosition();

template
int HSwitch<int16_t>::GetPosition();

template
int HSwitch<int32_t>::GetPosition();

// Add()
template
void HSwitch<int8_t>::Add(HReader<int8_t>* reader);

template
void HSwitch<uint8_t>::Add(HReader<uint8_t>* reader);

template
void HSwitch<int16_t>::Add(HReader<int16_t>* reader);

template
void HSwitch<int32_t>::Add(HReader<int32_t>* reader);

template
void HSwitch<int8_t>::Add(HWriter<int8_t>* writer);

template
void HSwitch<uint8_t>::Add(HWriter<uint8_t>* writer);

template
void HSwitch<int16_t>::Add(HWriter<int16_t>* writer);

template
void HSwitch<int32_t>::Add(HWriter<int32_t>* writer);

//! @endcond
#endif