#ifndef __HOBJECT_H
#define __HOBJECT_H

#include "hmetrics.h"
#include <string>

class HObject
{
    public:

        HMetrics Metrics;

        HObject();

        std::string GetMetrics(std::string id);
};

#endif