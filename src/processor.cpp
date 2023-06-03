#include "processor.h"
#include "linux_parser.h"

Processor::Processor() : prev_idle_{LinuxParser::IdleJiffies()},prev_active_{LinuxParser::ActiveJiffies()}, prev_total_{prev_active_ + prev_idle_} {}  
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  long curr_active = LinuxParser::ActiveJiffies();
  long curr_idle = LinuxParser::IdleJiffies();
  long total = curr_active + curr_idle;
  float util = static_cast<float>(curr_active - prev_active_) / (total - prev_total_); 
  prev_active_ = curr_active;
  prev_total_ = total;
  return util;
}
