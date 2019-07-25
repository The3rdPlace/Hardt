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

        HMetrics Metrics;

        HObject();

        std::string GetMetrics(std::string id);
        void ResetMetrics();
};

#endif