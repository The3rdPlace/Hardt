#ifndef __HPROBE_H
#define __HPROBE_H

#include "hfilewriter.h"

/**
    Dump pcm data to probe file for later evaluation
*/
template <class T>
class HProbe: public HFileWriter<T>
{
    private:

        bool _enabled;

    public:

        /** Initialize probe, writes only occurs when 'enable'
            is set to true */
        HProbe(std::string name, bool enable):
            HFileWriter<T>(name, "PROBE_" + name + ".pcm"),
            _enabled(enable)
        {
            if( _enabled )
            {
                HFileWriter<T>::Start();
            }
        }

        /** Stop the probe */
        ~HProbe()
        {
            if( _enabled )
            {
                HFileWriter<T>::Stop();
            }
        }

        /** Write to the probe file. Write errors is silently ignored! */
        int Write(T* src, size_t blocksize)
        {
            return HFileWriter<T>::Write(src, blocksize);
        }
};

#endif
