#include "hobject.h"

HObject::HObject():
    Metrics()
{}

std::string HObject::GetMetrics(std::string id)
{
    std::string output = "Metrics [" + id + "]:";
    output += " BytesOut=" + std::to_string(Metrics.BytesOut);
    output += " BytesIn=" + std::to_string(Metrics.BytesIn);
    output += " BlocksOut=" + std::to_string(Metrics.BlocksOut);
    output += " BlocksIn=" + std::to_string(Metrics.BlocksIn);
    output += " Writes=" + std::to_string(Metrics.Writes);
    output += " Reads=" + std::to_string(Metrics.Reads);
    return output;
}