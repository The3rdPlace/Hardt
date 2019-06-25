#ifndef __HTIMER_CPP
#define __HTIMER_CPP

#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>

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
    struct tm tm = {0};
    if (!strptime(datetime, "%Y-%m-%d %H:%M", &tm)) {
        HError("Incorrect datetime string %s", datetime);
    }
    //tm.tm_hour -= tm.tm_gmtoff;
    //tm.tm_hour -= (tm.tm_isdst > 0 ? 1 : 0);
    tm.tm_hour--;  // Works in denmark - we need to figure this out!
    std::cout << "gm " << tm.tm_gmtoff << " dst " << tm.tm_isdst << std::endl;
    HLog("datetime was %s, parsed time is %s", datetime, asctime(&tm));
    return mktime(&tm);
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
    if( !_active )
    {
        HLog("No timer set");
        return;
    }

    if( time(0) < _start )
    {
        struct tm* tm = localtime(&_start);
        HLog("Waiting untill %s", asctime(tm));

        std::chrono::seconds sleepDuration(1);
        while( _active && time(0) < _start )
        {
            std::this_thread::sleep_for(sleepDuration);
            std::cout << "now = " << _start - time(0) << std::endl;
        }

        tm = localtime(&_start);
        HLog("Wait finished, operation should run untill %s", asctime(tm));
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

#endif