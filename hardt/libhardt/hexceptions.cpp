#include "hexceptions.h"

HException::HException(const char* reason, const char* type):
    _why(reason),
    _type(type)
{
}

const char* HException::what() const throw()
{
    return _why;
}

const char* HException::type() const throw()
{
    return _type;
}

HInitializationException::HInitializationException(const char* reason):
    HException(reason, "HInitializationException")
{}

HNetworkException::HNetworkException(const char* reason):
    HException(reason, "HNetworkException")
{}

HFileIOException::HFileIOException(const char* reason):
    HException(reason, "HFileIOException")
{}

HAudioIOException::HAudioIOException(const char* reason):
    HException(reason, "HAudioIOException")
{}

HNotImplementedException::HNotImplementedException(const char* reason):
    HException(reason, "HNotImplementedException")
{}

HWriterIOException::HWriterIOException(const char* reason):
    HException(reason, "HWriterIOException")
{}

HReaderIOException::HReaderIOException(const char* reason):
    HException(reason, "HReaderIOException")
{}

HConverterIOException::HConverterIOException(const char* reason):
    HException(reason, "HConverterIOException")
{}

HFilterIOException::HFilterIOException(const char* reason):
    HException(reason, "HFilterIOException")
{}

HFilterInitializationException::HFilterInitializationException(const char* reason):
    HException(reason, "HFilterInitializationException")
{}

HInvalidArgumentException::HInvalidArgumentException(const char* reason):
    HException(reason, "HInvalidArgumentException")
{}

HUnknownCommandOpcodeException::HUnknownCommandOpcodeException(const char* reason):
        HException(reason, "HUnknownCommandOpcodeException")
{}
