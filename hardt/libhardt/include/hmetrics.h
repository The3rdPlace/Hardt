#ifndef __HMETRICS_H
#define __HMETRICS_H

#include <iostream>

static int metrics_id_cnt = 0;

/**
    Metrics data.

    The metrics is provided by all types derived from HObject ,
    which almost all types are.

    Updating the metrics must be done by the implementing class
    but currently, not all readers and writers do this - however
    processors _do_ update metrics and since they are the hubs that
    allows data to flow, measuring metrics there should be sufficient.
*/
class HMetrics
{
    public:

        /** Bytes (not samples) written to the next writer */
        unsigned long BytesOut;

        /** Bytes (not samples) read from the previous reader*/
        unsigned long BytesIn;

        /** Blocks written to the next writer*/
        unsigned long BlocksOut;

        /** Blocks read from the previous reader */
        unsigned long BlocksIn;

        /** How many times Write() was called on the writer */
        unsigned long Writes;

        /** How many times Read() was called on the reader */
        unsigned long Reads;

    public:

        /** Initialize all members of a metrics object */
        HMetrics():
            BytesOut(0),
            BytesIn(0),
            BlocksOut(0),
            BlocksIn(0),
            Writes(0),
            Reads(0) {}

        /** Return a string with the metrics information */
        std::string ToString()
        {
            std::string output = "";
            output += "  BytesOut=" + std::to_string(BytesOut) + "\n";
            output += "  BytesIn=" + std::to_string(BytesIn) + "\n";
            output += "  BlocksOut=" + std::to_string(BlocksOut) + "\n";
            output += "  BlocksIn=" + std::to_string(BlocksIn) + "\n";
            output += "  Writes=" + std::to_string(Writes) + "\n";
            output += "  Reads=" + std::to_string(Reads);
            return output;
        }

};


#endif