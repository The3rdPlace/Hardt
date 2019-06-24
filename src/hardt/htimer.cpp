#ifndef __HTIMER_CPP
#define __HTIMER_CPP

#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>

#include "htimer.h"

HTimer::HTimer():
    _active(false)
{
    _start = time(0);
    _stop = time(0);
}

HTimer::HTimer(char* stop):
    _active(true)
{
    _start = time(0);
    _stop = parseDateTime(stop);
}

HTimer::HTimer(char* start, char* stop):
    _active(true)
{
    // Start value
    _start = parseDateTime(start);
    _stop = parseDateTime(stop);

}

time_t HTimer::parseDateTime(char *datetime)
{
    std::cout << "parseDateTime ==> " << datetime << std::endl;

    // This function uses old c-style string splitting/parsing. More clever
    // methods exists, using std::getline etc., but this is simple, portable and readable.

    time_t now = time(0);
    struct tm* timeInfo = localtime( &now );

    if( datetime != NULL && (strlen(datetime) == 8 || strlen(datetime) == 19) )
    {
        int pos = 0;
        char valueString[5];

/*

https://stackoverflow.com/questions/39673816/want-to-perform-date-time-value-manipulation-using-struct-tm

struct tm tm = {0};
    if (!strptime("24-9-2016 13:30", "%d-%m-%Y %H:%M", &tm)) {
        return EXIT_FAILURE;
    }

    tm.tm_hour += 5;
    tm.tm_isdst = -1;
    mktime(&tm);

    char buf[40];
    if (!strftime(buf, sizeof buf, "%d-%m-%Y %H:%M", &tm)) {
        return EXIT_FAILURE;
    }
*/
        // YYYY-MM-DD HH:MM:SS
        if( strlen(datetime) == 19 )
        {
            memcpy(valueString, &datetime[pos], 4);
            valueString[4] = 0;
            timeInfo->tm_year = atoi(valueString);
            pos += 5;

            memcpy(valueString, &datetime[pos], 2);
            valueString[2] = 0;
            timeInfo->tm_mon = atoi(valueString);
            timeInfo->tm_mon -= 1; // range: 0->11
            pos += 3;

            memcpy(valueString, &datetime[8], 2);
            valueString[2] = 0;
            timeInfo->tm_mday = atoi(valueString);
            pos += 3;
        }

        memcpy(valueString, &datetime[pos], 2);
        valueString[2] = 0;
        std::cout << "v " << valueString << std::endl;
        timeInfo->tm_hour = atoi(valueString);
        std::cout << "tm_hour " << timeInfo->tm_hour << std::endl;
        pos += 3;

        memcpy(valueString, &datetime[pos], 2);
        valueString[2] = 0;
        std::cout << "v " << valueString << std::endl;
        timeInfo->tm_min = atoi(valueString);
        pos += 3;

        memcpy(valueString, &datetime[pos], 2);
        valueString[2] = 0;
        std::cout << "v " << valueString << std::endl;
        timeInfo->tm_sec = atoi(valueString);
        pos += 3;
    }

    std::cout << "parseDateTime <== " << asctime(timeInfo);
    return mktime( timeInfo );
}

void HTimer::setStart(char *datetime)
{
    _start = parseDateTime(datetime);
    std::cout << "start = " << _start << std::endl;
    _active = true;
}

void HTimer::setStop(char *datetime)
{
    _stop = parseDateTime(datetime);
    std::cout << "stop = " << _stop << std::endl;
    _active = true;
}

void HTimer::wait()
{
    std::cout << "wait start" << std::endl;
    std::chrono::seconds sleepDuration(1);
    while( _active && time(0) < _start )
    {
        std::this_thread::sleep_for(sleepDuration);
        std::cout << "now = " << time(0) << std::endl;
    }
    std::cout << "wait stop" << std::endl;
}

#endif