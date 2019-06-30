#ifndef __HTIMER_CPP
#define __HTIMER_CPP

#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>
#include <string.h>

#include "hardt.h"
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
    // Check that we know the datetime format
    if( strlen(datetime) != 5 && strlen(datetime) != 16 )
    {
        HError("Unknown datetime string format");
        return 0;
    }

    // Initialize with 'now'
    time_t now = time(0);
    struct tm* tm = localtime(&now);
    tm->tm_sec = 0;

    // Parse input
    if (!strptime(datetime, strlen(datetime) == 5 ? "%H:%M" : "%Y-%m-%d %H:%M", tm)) {
        HError("Incorrect datetime string %s", datetime);
    }
    HLog("Datetime was %s, parsed time is %s", datetime, HTimer::asctime2(tm));

    // Return timestamp
    time_t result = mktime(tm);
    HLog("Returned timestamp is %s", HTimer::asctime2(localtime(&result)));
    return result;
}

void HTimer::setStart(char *datetime)
{
    _start = parseDateTime(datetime);
    _active = true;
}

void HTimer::setStop(char *datetime)
{
    _stop = parseDateTime(datetime);
    _active = true;
}

void HTimer::wait()
{
    if( !_active )
    {
        HLog("No timer set");
        return;
    }

    if( time(0) < _start )
    {
        struct tm* tm = localtime(&_start);
        HLog("Waiting untill %s", HTimer::asctime2(tm));

        std::chrono::seconds sleepDuration(1);
        while( _active && time(0) < _start )
        {
            std::this_thread::sleep_for(sleepDuration);
        }

        tm = localtime(&_stop);
        HLog("Wait finished, operation should run untill %s", HTimer::asctime2(tm));
    }
    else if( time(0) > _stop )
    {
        HLog("Beyond stop time");
    }
    else
    {
        HLog("After start time and not beyound stop time, no wait");
    }
}

char *HTimer::asctime2(struct tm* tm)
{
    char *time = asctime(tm);
    #ifdef _WIN32
        time[strlen(time) - 2] = '\0';
    #else
        time[strlen(time) - 1] = '\0';
    #endif

    return time;
}
#endif