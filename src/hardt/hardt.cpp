#include "hardtapi.h"

#include <stdio.h>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <stdarg.h>

std::string getversion()
{
    std::stringstream ss;
    ss << "version " << MAJORVERSION << "." << MINORVERSION << "." << BUILDNO;
    return ss.str();
}

/********************************************************************
Logging
********************************************************************/

std::string LogStreamName = "";
std::string ErrorStreamName = "";

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

std::string getFormattedMessage(const char* fmt, va_list args)
{
    char output[256];

    vsnprintf(output, 256, fmt, args);
    return std::string(output);
}

void WriteToStream(std::string streamName, std::string output)
{
    if( streamName != "" )
    {
        std::ofstream stream;
        stream.open(streamName, std::ios::out | std::ios::app);
        stream << output << std::endl;
        stream.close();
    }
    else
    {
        std::cout << output << std::endl;
    }

}

void HWriteLogMessage(std::string file, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    std::string message = getFormattedMessage(fmt, args);
    va_end(args);

    std::string output = getFilenameWithoutPath(file, true) + std::string(": ") + message;
    WriteToStream(LogStreamName, output);
}

void HWriteErrorMessage(std::string file, std::string line, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    std::string message = getFormattedMessage(fmt, args);
    va_end(args);

    std::string output = "ERROR at " + getFilenameWithoutPath(file, false) + std::string("@") + line + std::string(": ") + message;
    WriteToStream(ErrorStreamName, output);
}

void HInit(std::string logname, bool verbose)
{
    if( !verbose )
    {
        // Set names for log- and error files
        LogStreamName = logname + ".log";
        ErrorStreamName = logname + ".err";
    }

    // Redirect stderr to a logfile
    freopen((logname + ".stderr").c_str(), "w", stderr);
}
