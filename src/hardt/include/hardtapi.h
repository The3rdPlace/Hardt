#ifndef __HARDTAPI_H
#define __HARDTAPI_H

/********************************************************************
Abstract base classes with no, or little implementation - so they
exists here since they form the base of the entire toolkit.
********************************************************************/

template <class T>
class HWriter
{
    public:

        virtual int Write(T* src, int size) = 0;
        virtual int Blocksize() = 0;
};

template <class T>
class HReader
{
    public:

        virtual int Read(T* dest) = 0;
        virtual int Blocksize() = 0;
};

/********************************************************************
Include api function declarations
********************************************************************/

#include <portaudio.h>
#include "hardt.h"
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


/********************************************************************
Generic highlevel sample reader
********************************************************************/
/*template <class T>
class HSampleReader
{
    public:

            HSampleReader(HReader* reader);
            int Read(T* dest, int size);
};*/

#endif
