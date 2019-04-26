#ifndef __HMETRICS_H
#define __HMETRICS_H

struct HMetrics
{
    unsigned long BytesOut;
    unsigned long BytesIn;
    unsigned long BlocksOut;
    unsigned long BlocksIn;
    unsigned long Writes;
    unsigned long Reads;
};

#endif