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

std::string getFilenameWithoutPath(std::string pathAndFilename, bool removeExtension)
{
    #ifdef _WIN32
    std::string pathSeparator("\\");
    #else
    std::string pathSeparator("/");
    #endif

    std::size_t loc = pathAndFilename.rfind(pathSeparator);
    if( loc !=  std::string::npos)
    {
        std::string filename = pathAndFilename.substr(loc + 1, std::string::npos);
        loc = filename.find(std::string("."));
        if( removeExtension && loc !=  std::string::npos)
        {
            return filename.substr(0, loc);
        }
        else
        {
            return filename;
        }
    }
    return pathAndFilename;
}

void writeToStream(std::ofstream stream, std::string msg)
{
    stream << msg << std::endl;
}

void HWriteLogMessage(std::string file, std::string message)
{
    std::string output = getFilenameWithoutPath(file, true) + std::string(": ") + message;
    if( HLogStream.is_open() )
    {
        HLogStream << output << std::endl;
    }
    else
    {
        std::cout << output << std::endl;
    }
}

void HWriteErrorMessage(std::string file, std::string line, std::string message)
{
    std::string output = "ERROR at " + getFilenameWithoutPath(file, false) + std::string("@") + line + std::string(": ") + message;
    if( HLogStream.is_open() )
    {
        HErrorStream << output << std::endl;
    }
    else
    {
        std::cerr << output << std::endl;
    }
}

void HInit(std::string logname)
{
    // Open standard log- and errorfiles
    HLogStream.open(logname + ".log");
    HErrorStream.open(logname + ".err");

    // Redirect stderr to a logfile
    freopen((logname + ".stderr").c_str(), "w", stderr);

    // Remaining initialization
    HInit();
}

void HInit()
{
    // No further initialization at the moment
}
