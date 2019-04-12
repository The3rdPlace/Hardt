#include "hardtapi.h"

#include <stdio.h>
#include <sstream>

std::string getversion()
{
    std::stringstream ss;
    ss << "version " << MAJORVERSION << "." << MINORVERSION << "." << BUILDNO;
    return ss.str();
}

