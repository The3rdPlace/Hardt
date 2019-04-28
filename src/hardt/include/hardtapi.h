#ifndef __HARDTAPI_H
#define __HARDTAPI_H

/********************************************************************
Initialization, logging and other basic functions.
(implemented in hardt.cpp)
********************************************************************/

#include "hardt.h"
#include "hexceptions.h"
#include "hsoundcard.h"
#include "hwav.h"
#include "hmetrics.h"
#include "hobject.h"

/********************************************************************
Abstract base classes with no, or little implementation
********************************************************************/

template <class T>
class HWriter : public HObject
{
    public:

        virtual int Write(T* src, size_t blocksize) = 0;

        virtual bool Start(void* data) { return true; }
        virtual bool Stop() { return true; }
};

template <class T>
class HReader : public HObject
{
    public:

        virtual int Read(T* dest, size_t blocksize) = 0;

        virtual bool Start(void* data) { return true; }
        virtual bool Stop() { return true; }
};

template <class T>
class HProcessor : public HObject
{
    protected:

        HWriter<T>* _writer;
        HReader<T>* _reader;

    public:

        HProcessor(HWriter<T>* writer, HReader<T>* reader):
            _writer(writer),
            _reader(reader)
        {}

        int Read(T* dest, int blocksize)
        {
            try
            {
                return _reader->Read(dest, blocksize);
            }
            catch(std::exception e)
            {
                HError("Exception in Write(): %s", e.what());
                return 0;
            }
        }

        int Write(T* src, int blocksize)
        {
            try
            {
                return _writer->Write(src, blocksize);
            }
            catch(std::exception e)
            {
                HError("Exception in Read(): %s", e.what());
                return 0;
            }
        }

        bool Start(void* data)
        {
            return _writer->Start(data) && _reader->Start(data);
        }

        bool Stop()
        {
            return _writer->Stop() && _reader->Stop();
        }

        void SetReader(HReader<T>* reader)
        {
            _reader = reader;
        }

        void SetWriter(HWriter<T>* writer)
        {
            _writer = writer;
        }

        virtual void Run() = 0;
};

/********************************************************************
Include api function declarations
********************************************************************/

//#include "hsoundcard.h"
#include "hsoundcardreader.h"
#include "hfilewriter.h"
#include "hwavwriter.h"
#include "hwavreader.h"
#include "hnetworkwriter.h"
#include "hnetworkreader.h"
#include "hnetworkprocessor.h"
//#include "hwav.h"

#endif
