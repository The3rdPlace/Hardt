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

        /** Contains the exception type, if one needs to find out which exception is being thrown */
        const char* _type;

    public:

        /** Construct a new HException object */
        HException(const char* reason, const char* type);

        /** Return the exception reason */
        const char* what() const throw();

        /** Return the exception type */
        const char* type() const throw();
};

/**
    Thrown by constructors if the object could not be initialized

    Constructor:
        HInitializationException(reason)
*/
class HInitializationException : public HException
{
    public:

        /** Construct a new HInitializationException with a reason */
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

        /** Construct a new HNetworkException with a reason */
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

        /** Construct a new HFileIOException with a reason */
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

        /** Construct a new HAudioIOException with a reason */
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
    Thrown if there is errors during reader i/o

    Constructor:
        HReaderIOException(reason)
*/
class HReaderIOException : public HException
{
    public:

        /** Construct a new HWriterIOException */
        HReaderIOException(const char* reason);
};

/**
    Thrown if there is errors during converter i/o

    Constructor:
        HWConverterIOException(reason)
*/
class HConverterIOException : public HException
{
    public:

        /** Construct a new HConverterIOException */
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

        /** Construct a new HFilterIOException with a reason */
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

        /** Construct a new HFilterInitializationException with a reason */
        HFilterInitializationException(const char* reason);
};

/**
    Thrown if an invalid argument is given

    Constructor:
        HInvalidArgumentException(reason)
*/
class HInvalidArgumentException : public HException
{
    public:

        /** Construct a new HInvalidArgumentException with a reason */
        HInvalidArgumentException(const char* reason);
};

/**
    Thrown if an invalid command opcode is given

    Constructor:
        HUnknownCommandOpcodeException(reason)
*/
class HUnknownCommandOpcodeException : public HException
{
public:

    /** Construct a new HUnknownCommandOpcodeException with a reason */
    HUnknownCommandOpcodeException(const char* reason);
};

#endif
