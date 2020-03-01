#include "hexceptions.h"

HException::HException(const char* reason):
    _why(reason)
{
}

const char* HException::what() const throw()
{
    return _why;
}

HInitializationException::HInitializationException(const char* reason):
    HException(reason)
{}

HNetworkException::HNetworkException(const char* reason):
    HException(reason)
{}

HFileIOException::HFileIOException(const char* reason):
    HException(reason)
{}

HAudioIOException::HAudioIOException(const char* reason):
    HException(reason)
{}

HNotImplementedException::HNotImplementedException(const char* reason):
    HException(reason)
{}

HWriterIOException::HWriterIOException(const char* reason):
    HException(reason)
{}

HReaderIOException::HReaderIOException(const char* reason):
    HException(reason)
{}

HConverterIOException::HConverterIOException(const char* reason):
    HException(reason)
{}

HFilterIOException::HFilterIOException(const char* reason):
    HException(reason)
{}

HFilterInitializationException::HFilterInitializationException(const char* reason):
    HException(reason)
{}

HInvalidArgumentException::HInvalidArgumentException(const char* reason):
    HException(reason)
{}

HUnknownCommandOpcodeException::HUnknownCommandOpcodeException(const char* reason):
        HException(reason)
{}
