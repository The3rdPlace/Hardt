#ifndef __HARDTAPI_H
#define __HARDTAPI_H

/********************************************************************
Initialization, logging and other basic functions.
(implemented in hardt.cpp)
********************************************************************/

#include "hardt.h"

/********************************************************************
Abstract base classes with no, or little implementation
********************************************************************/

template <class T>
class HWriter
{
    public:

        virtual int Write(T* src, int size) = 0;
        virtual int Blocksize() = 0;
        virtual bool Start() { return true; }
        virtual bool Stop() { return true; }
};

template <class T>
class HReader
{
    public:

        virtual int Read(T* dest) = 0;
        virtual int Blocksize() = 0;
        virtual bool Start() { return true; }
        virtual bool Stop() { return true; }
};

/********************************************************************
Include api function declarations
********************************************************************/

#include <portaudio.h>
#include "hnetworkserver.h"
#include "hsoundcardreader.h"

/********************************************************************
Network classes
********************************************************************/

/*class HNetworkClient : public HReader
{
    private:

        int _socket;
        bool _connected;

    public:

        int Read(void* dest, int size);
};*/

#endif
