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

HFilterIOException::HFilterIOException(const char* reason):
    HException(reason)
{}
