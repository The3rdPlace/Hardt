#ifndef __HTIMER_H
#define __HTIMER_H

/**
    Timer that will wait to the given date and time
*/
class HTimer
{
    private:

        time_t _start;
        time_t _stop;

        time_t ParseDateTime(char *datetime);

        static char *Asctime2(struct tm* tm);

        bool _active;

    public:

        /**
            Create a new timer
        */
        HTimer();

        /**
            Create a new timer with a start date and time of NOW and a given stop date and time.
            The stop definition has the format "%HH:%MM" or "%YYYY-%mm-%dd %HH:%MM" (H = Hour, M = minute, Y = Year, m = month, d = day)
        */
        HTimer(char* stop);

        /**
            Create a new timer with a given start and stop date and time
            The start and stop definition has the format "%HH:%MM" or "%YYYY-%mm-%dd %HH:%MM" (H = Hour, M = minute, Y = Year, m = month, d = day)
        */
        HTimer(char* start, char* stop);

        /**
            Create a new timer that waits for a given duration (seconds)
        */
        HTimer(long duration)
        {
            _start = time(0);
            _stop = _start + duration;
        }

        /**
            Set a (new) start date and time
            The datetime definition has the format "%HH:%MM" or "%YYYY-%mm-%dd %HH:%MM" (H = Hour, M = minute, Y = Year, m = month, d = day)
        */
        void SetStart(char* datetime);

        /**
            Set a (new) stop date and time
            The datetime definition has the format "%HH:%MM" or "%YYYY-%mm-%dd %HH:%MM" (H = Hour, M = minute, Y = Year, m = month, d = day)
        */
        void SetStop(char* datetime);

        /**
            Return the duration left before the timer continues
        */
        long Duration()
        {
            if( !_active )
            {
                return 0;
            }
            time_t now = time(0);
            return _stop - now;
        }

        /**
            Returns true if the timer is currently active, thus would not block in Wait()
        */
        bool Active()
        {
            time_t now = time(0);
            return _active && now > _start && now < _stop;
        }

        /**
            Returns true if the timer is currently active and the start time has not yet arrived, thus would block in Wait()
        */
        bool Before()
        {
            time_t now = time(0);
            return _active && now < _start;
        }

        /**
            Returns true if the timer is currently active and the stop time has arrived, thus would not block in Wait()
        */
        bool After()
        {
            time_t now = time(0);
            return _active && now > _stop;
        }

        /**
            Block untill the timer expires
        */
        void Wait();

        /**
            Return the current start date and time
        */
        time_t Start()
        {
            return _start;
        }

        /**
            Return the current stop date and time
        */
        time_t Stop()
        {
            return _stop;
        }
};

#endif