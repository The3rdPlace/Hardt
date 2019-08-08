#ifndef __HEXCEPTIONS_H
#define __HEXCEPTIONS_H

#include <stdlib.h>
#include <exception>

/**
    Base class for specific exceptions thrown by any Hardt class.
    This type can not be instantianted directly.
*/
class HException : public std::exception
{
    protected:

        /** Contains the exception reason */
        const char* _why;

    public:

        /** Construct a new HException object */
        HException(const char* reason);

        /** Return the exception reason */
        const char* what() const throw();
};

/**
    Thrown by constructors if the object could not be initialized

    Constructor:
        HInitializationException(reason)
*/
class HInitializationException : public HException
{
    public:

        HInitializationException(const char* reason);
};

/**
    Thrown if a network connection fails

    Constructor:
        HNetworkException(reason)
*/
class HNetworkException : public HException
{
    public:

        HNetworkException(const char* reason);
};

/**
    Thrown if there is errors during file i/o

    Constructor:
        HFileIOException(reason)
*/
class HFileIOException : public HException
{
    public:

        HFileIOException(const char* reason);
};

/**
    Thrown if there is errors during audio i/o

    Constructor:
        HAudioIOException(reason)
*/
class HAudioIOException : public HException
{
    public:

        HAudioIOException(const char* reason);
};

/**
    Thrown by functions if they have not been implemneted

    Constructor:
        HNotImplementedException()
*/
class HNotImplementedException : public HException
{
    public:

        /** Construct a new HNotImplementedException with a resason */
        HNotImplementedException(const char* reason = "Not implemented");
};

/**
    Thrown if there is errors during writer i/o

    Constructor:
        HWriterIOException(reason)
*/
class HWriterIOException : public HException
{
    public:

        /** Construct a new HWriterIOException */
        HWriterIOException(const char* reason);
};

/**
    Thrown if there is errors during converter i/o

    Constructor:
        HWConverterIOException(reason)
*/
class HConverterIOException : public HException
{
    public:

        HConverterIOException(const char* reason);
};

/**
    Thrown if there is errors during filter i/o

    Constructor:
        HFilterIOException(reason)
*/
class HFilterIOException : public HException
{
    public:

        HFilterIOException(const char* reason);
};

/**
    Thrown if there is errors during filter initialization

    Constructor:
        HFilterInitializationException(reason)
*/
class HFilterInitializationException : public HException
{
    public:

        HFilterInitializationException(const char* reason);
};

#endif
