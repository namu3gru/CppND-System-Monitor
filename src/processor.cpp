#include "processor.h"
#include "linux_parser.h"
#include <iostream>

// TODO: Return the aggregate CPU utilization
Processor::Processor(){}
float Processor::Utilization() {
    long idle = LinuxParser::IdleJiffies();
    long total = LinuxParser::Jiffies();
    long diff_idle = idle - this->prevIdle;
    long diff_total = total - this->prevTotal;
    float diff_usage = (10*(diff_total-diff_idle)/diff_total)/(float)10;
    this->prevIdle = idle;
    this->prevTotal = total;
    return diff_usage;
}
