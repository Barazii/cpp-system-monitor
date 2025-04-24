#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization()
{
    long total = LinuxParser::Jiffies();
    long idle = LinuxParser::IdleJiffies();

    long totalDelta = total - prevTotal_;
    long idleDelta = idle - prevIdle_;

    prevTotal_ = total;
    prevIdle_ = idle;

    return totalDelta == 0 ? 0.0 : (total - idle) / (float)total;
}