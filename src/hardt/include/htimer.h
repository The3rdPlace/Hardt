#ifndef __HTIMER_H
#define __HTIMER_H

class HTimer
{
    private:

        time_t _start;
        time_t _stop;

        time_t parseDateTime(char *datetime);

        bool _active;

    public:

        HTimer();

        HTimer(char* stop);

        HTimer(char* start, char* stop);

        HTimer(long duration)
        {
            _start = time(0);
            _stop = _start + duration;
        }

        void setStart(char* datetime);

        void setStop(char* datetime);

        long duration()
        {
            if( !_active )
            {
                return 0;
            }
            time_t now = time(0);
            return _stop - now;
        }

        bool active()
        {
            time_t now = time(0);
            return _active && now > _start && now < _stop;
        }

        void wait();

        time_t start()
        {
            return _start;
        }

        time_t stop()
        {
            return _stop;
        }
};

#endif