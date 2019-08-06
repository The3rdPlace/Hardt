#ifndef __HOBJECT_H
#define __HOBJECT_H

#include "hmetrics.h"
#include <string>

/**
    Base class for (almost) all Hardt types.

    If you extend the Hardt framework by adding your own components,
    then either inherit an existing Hardt component, or at the very least
    this type.
*/
class HObject
{
    public:

        /** The metrics object */
        HMetrics Metrics;

        /** Construct a new HObject */
        HObject();

        /** Get metrics details as a formatted output string */
        std::string GetMetrics(std::string id);

        /** Reset all metrics */
        void ResetMetrics();
};

#endif