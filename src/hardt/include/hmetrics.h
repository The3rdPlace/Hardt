#ifndef __HMETRICS_H
#define __HMETRICS_H

/**
    Metrics data.

    The metrics is provided by all types derived from HObject ,
    which almost all types are.

    Updating the metrics must be done by the implementing class
    but currently, not all readers and writers do this - however
    processors _do_ update metrics and since they are the hubs that
    allows data to flow, measuring metrics there should be sufficient.
*/
struct HMetrics
{
    /** Bytes (not samples) written */
    unsigned long BytesOut;

    /** Bytes (not samples) read */
    unsigned long BytesIn;

    /** Blocks written */
    unsigned long BlocksOut;

    /** Blocks read */
    unsigned long BlocksIn;

    /** Writes */
    unsigned long Writes;

    /** Reads */
    unsigned long Reads;
};

#endif