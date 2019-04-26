#include "hobject.h"

HObject::HObject():
    Metrics()
{}

std::string HObject::GetMetrics(std::string id)
{
    std::string output = "Metrics [" + id + "]:" + "\n";
    output += "  BytesOut=" + std::to_string(Metrics.BytesOut) + "\n";
    output += "  BytesIn=" + std::to_string(Metrics.BytesIn) + "\n";
    output += "  BlocksOut=" + std::to_string(Metrics.BlocksOut) + "\n";
    output += "  BlocksIn=" + std::to_string(Metrics.BlocksIn) + "\n";
    output += "  Writes=" + std::to_string(Metrics.Writes) + "\n";
    output += "  Reads=" + std::to_string(Metrics.Reads) + "\n";
    return output;
}

void HObject::ResetMetrics()
{
    Metrics = {};
}