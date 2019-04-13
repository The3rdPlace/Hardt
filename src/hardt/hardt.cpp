#include "hardtapi.h"

#include <stdio.h>
#include <sstream>
#include <fstream>

std::string getversion()
{
    std::stringstream ss;
    ss << "version " << MAJORVERSION << "." << MINORVERSION << "." << BUILDNO;
    return ss.str();
}

/********************************************************************
Logging
********************************************************************/

std::ofstream HLogStream;
std::ofstream HErrorStream;

void HWriteLogMessage(std::string message)
{
    if( HLogStream.is_open() )
    {
        HLogStream << message << std::endl;
    }
    else
    {
        std::cout << message << std::endl;
    }
}

void HWriteErrorMessage(std::string file, std::string line, std::string message)
{
    if( HLogStream.is_open() )
    {
        HErrorStream << message << std::endl;
    }
    else
    {
        std::cout << "ERROR:" << std::endl << file << "@" << line << std::endl << message << std::endl;
    }
}

void HInit(std::string logname)
{
    HLogStream.open(logname + ".log");
    HErrorStream.open(logname + ".err");
    freopen((logname + ".stderr").c_str(), "w", stderr);
}