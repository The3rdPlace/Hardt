#ifndef __HSWITCH_CPP
#define __HSWITCH_CPP

#include "hswitch.h"

template <class T>
HSwitch<T>::HSwitch(std::string id, HWriter<T>* writer, size_t blocksize):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _isReader(false),
    _isWriter(true),
    _isWriterConsumer(false),
    _position(0),
    _blocksize(blocksize),
    _writer(writer),
    _reader(NULL)
{
    HLog("HSwitch(HWriter*, blocksize=%d)", blocksize);
    _buffer = new T[_blocksize];
}

template <class T>
HSwitch<T>::HSwitch(std::string id, HWriterConsumer<T>* consumer, size_t blocksize):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _isReader(false),
    _isWriter(true),
    _isWriterConsumer(true),
    _position(0),
    _blocksize(blocksize),
    _writer((HWriter<T>*) consumer),
    _reader(NULL)
{
    HLog("HSwitch(HWriterConsumer*, blocksize=%d)", blocksize);
    _buffer = new T[_blocksize];
    consumer->SetWriter(this);

}

template <class T>
HSwitch<T>::HSwitch(std::string id, HReader<T>* reader, size_t blocksize):
    HReader<T>(id),
    HWriter<T>(id),
    HWriterConsumer<T>(id),
    _isReader(true),
    _isWriter(false),
    _isWriterConsumer(false),
    _position(0),
    _blocksize(blocksize),
    _writer(NULL),
    _reader(reader)
{
    HLog("HSwitch(HReader*, blocksize=%d)", blocksize);
    _buffer = new T[_blocksize];
}

template <class T>
HSwitch<T>::~HSwitch()
{
    delete[] _buffer;
}

template <class T>
int HSwitch<T>::Write(T* src, size_t blocksize)
{
    if( !_isWriter )
    {
        throw new HInitializationException("This HSwitch is not a writer");
    }

    if( blocksize > _blocksize )
    {
        HError("Requested blocksize for write is too big: %d requested, max is %d", blocksize, _blocksize);
        throw new HWriterIOException("Requested blocksize for write is too big");
    }

    if( _position == 0 )
    {
        return _writer->Write(src, blocksize);
    }

    return _components.at(_position - 1).Writer->Write(src, blocksize);
}

template <class T>
int HSwitch<T>::ReadImpl(T* dest, size_t blocksize)
{
    if( !_isReader )
    {
        throw new HInitializationException("This HSwitch is not a reader");
    }

    if( blocksize > _blocksize )
    {
        HError("Requested blocksize for read is too big: %d requested, max is %d", blocksize, _blocksize);
        throw new HReaderIOException("Requested blocksize for read is too big");
    }

    if( _position == 0 )
    {
        return _reader->Read(dest, blocksize);
    }

    return _components.at(_position - 1).Reader->Read(dest, blocksize);
}

template <class T>
void HSwitch<T>::SetPosition(int position)
{
    if( position > _components.size() )
    {
        HError("Requested switch position is higher than the number of available components = %d", _components.size());
        throw new HInvalidArgumentException("Requested switch position is higher than the number of available components");
    }
    if( position < 0 )
    {
        HError("Requested switch position is below zero", _components.size());
        throw new HInvalidArgumentException("Requested switch position is below zero");
    }
    HLog("Switch set to position %d", position);
    _position = position;
}

template <class T>
int HSwitch<T>::GetPosition()
{
    return _position;
}

template <class T>
void HSwitch<T>::Add(HReader<T>* reader)
{
	if( !_isReader && _isWriter )
	{
	    HError("A HReader can not be added to a HSwitch initialized with HWriter's");
        throw new HInitializationException("A HWriter can not be added to a HSwitch initialized with HWriter's");
	}
	_isReader = true;
	_components.push_back(ComponentPtr(reader));
}

template <class T>
void HSwitch<T>::Add(HWriter<T>* writer)
{
	if( _isReader && !_isWriter )
	{
	    HError("A HWriter can not be added to a HSwitch initialized with HReader's");
        throw new HInitializationException("A HWriter can not be added to a HSwitch initialized with HReader's");
	}
	_isWriter = true;
	_components.push_back(HSwitch<T>::ComponentPtr(writer));
}

template <class T>
void HSwitch<T>::SetWriter(HWriter<T>* writer)
{
	if( !_isWriterConsumer )
	{
	    HError("It is not possible to set a writer on a HSwitch initialized with HReader's or HWriter's (hint: use a HWriterConsumer*)");
        throw new HInitializationException("It is not possible to set a writer on a HSwitch initialized with HReader's or HWriter's (hint: use a HWriterConsumer*)");
	}

    HLog("Writer is upstream (we hope), setting new writer");
    _writer = writer;
}

template <class T>
bool HSwitch<T>::Start()
{
    for (auto& component: _components)
    {
        if( _isWriter)
        {
            if( !component.Writer->Start() )
            {
                return false;
            }
        }
        if( _isReader)
        {
            if( !component.Reader->Start() )
            {
                return false;
            }
        }
    }
    return true;
}

template <class T>
bool HSwitch<T>::Stop()
{
    for (auto& component: _components)
    {
        if( _isWriter)
        {
            if( !component.Writer->Stop() )
            {
                return false;
            }
        }
        if( _isReader)
        {
            if( !component.Reader->Stop() )
            {
                return false;
            }
        }
    }
    return true;
}

/********************************************************************
Explicit instantiation
********************************************************************/
//! @cond Doxygen_Suppress

// HSwitch()
template
HSwitch<int8_t>::HSwitch(std::string id, HWriter<int8_t>* writer, size_t blocksize);

template
HSwitch<uint8_t>::HSwitch(std::string id, HWriter<uint8_t>* writer, size_t blocksize);

template
HSwitch<int16_t>::HSwitch(std::string id, HWriter<int16_t>* writer, size_t blocksize);

template
HSwitch<int32_t>::HSwitch(std::string id, HWriter<int32_t>* writer, size_t blocksize);

template
HSwitch<int8_t>::HSwitch(std::string id, HWriterConsumer<int8_t>* consumer, size_t blocksize);

template
HSwitch<uint8_t>::HSwitch(std::string id, HWriterConsumer<uint8_t>* consumer, size_t blocksize);

template
HSwitch<int16_t>::HSwitch(std::string id, HWriterConsumer<int16_t>* consumer, size_t blocksize);

template
HSwitch<int32_t>::HSwitch(std::string id, HWriterConsumer<int32_t>* consumer, size_t blocksize);

template
HSwitch<int8_t>::HSwitch(std::string id, HReader<int8_t>* reader, size_t blocksize);

template
HSwitch<uint8_t>::HSwitch(std::string id, HReader<uint8_t>* reader, size_t blocksize);

template
HSwitch<int16_t>::HSwitch(std::string id, HReader<int16_t>* reader, size_t blocksize);

template
HSwitch<int32_t>::HSwitch(std::string id, HReader<int32_t>* reader, size_t blocksize);

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
int HSwitch<int8_t>::ReadImpl(int8_t* dest, size_t blocksize);

template
int HSwitch<uint8_t>::ReadImpl(uint8_t* dest, size_t blocksize);

template
int HSwitch<int16_t>::ReadImpl(int16_t* dest, size_t blocksize);

template
int HSwitch<int32_t>::ReadImpl(int32_t* dest, size_t blocksize);

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

// SetWriter()
template
void HSwitch<int8_t>::SetWriter(HWriter<int8_t>* writer);

template
void HSwitch<uint8_t>::SetWriter(HWriter<uint8_t>* writer);

template
void HSwitch<int16_t>::SetWriter(HWriter<int16_t>* writer);

template
void HSwitch<int32_t>::SetWriter(HWriter<int32_t>* writer);

// Start()
template
bool HSwitch<int8_t>::Start();

template
bool HSwitch<uint8_t>::Start();

template
bool HSwitch<int16_t>::Start();

template
bool HSwitch<int32_t>::Start();

// Stop()
template
bool HSwitch<int8_t>::Stop();

template
bool HSwitch<uint8_t>::Stop();

template
bool HSwitch<int16_t>::Stop();

template
bool HSwitch<int32_t>::Stop();

//! @endcond
#endif
