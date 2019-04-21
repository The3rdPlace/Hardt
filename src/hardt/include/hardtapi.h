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

        virtual int Write(T* src, size_t blocksize) = 0;

        virtual bool Start(void* data) { return true; }
        virtual bool Stop() { return true; }
};

template <class T>
class HReader
{
    public:

        virtual int Read(T* dest, size_t blocksize) = 0;

        virtual bool Start(void* data) { return true; }
        virtual bool Stop() { return true; }
};

/********************************************************************
Exceptions
********************************************************************/

/*
    Thrown by constructors if the object could not be initialized

    Constructor:
        HInitializationException(reason)
*/
class HInitializationException : public std::exception
{
    private:

        const char* _why;

    public:

        HInitializationException(const char* reason): _why(reason) {}

        virtual const char* what() const throw()
        {
            return _why;
        }
};

/*
    Thrown if a network connection fails

    Constructor:
        HNetworkException(reason)
*/
class HNetworkException : public std::exception
{
    private:

        const char* _why;

    public:

        HNetworkException(const char* reason): _why(reason) {}

        virtual const char* what() const throw()
        {
            return _why;
        }
};

/*
    Thrown if there is errors during file i/o

    Constructor:
        HFileIOException(reason)
*/
class HFileIOException : public std::exception
{
    private:

        const char* _why;

    public:

        HFileIOException(const char* reason): _why(reason) {}

        virtual const char* what() const throw()
        {
            return _why;
        }
};

/*
    Thrown if there is errors during audio i/o

    Constructor:
        HAudioIOException(reason)
*/
class HAudioIOException : public std::exception
{
    private:

        const char* _why;

    public:

        HAudioIOException(const char* reason): _why(reason) {}

        virtual const char* what() const throw()
        {
            return _why;
        }
};

/*
    Thrown by functions if they have not been implemneted

    Constructor:
        HNotImplementedException()
*/
class HNotImplementedException : public std::exception
{
    public:

        HNotImplementedException() {}

        virtual const char* what() const throw()
        {
            return "Not implemented";
        }
};

/********************************************************************
Include api function declarations
********************************************************************/

#include "hsoundcard.h"
#include "hsoundcardreader.h"
#include "hfilewriter.h"
#include "hnetworkwriter.h"
#include "hnetworkreader.h"
#include "hnetwork.h"
#include "hnetworkserver.h"
#include "hnetworkclient.h"

#endif
