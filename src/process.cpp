#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid)  
: pid_{pid}, 
  user_{LinuxParser::User(pid)}, 
  command_{LinuxParser::Command(pid)}, 
  ram_{LinuxParser::Ram(pid)}, 
  uptime_{LinuxParser::UpTime(pid)} {} 

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization


float Process::CpuUtilization() {
  long total_time = LinuxParser::ActiveJiffies(pid_);
  long seconds = LinuxParser::UpTime() - uptime_;
  cpu_util_ = (float(total_time) / sysconf(_SC_CLK_TCK)) / float(seconds);
  return cpu_util_;
}



// void Process::Update() {
//     ram_ = LinuxParser::Ram(pid_);
//     uptime_ = LinuxParser::UpTime(pid_);
//     cpu_util_ = CpuUtilization();
// }

// TODO: Return the command that generated this process
string Process::Command() const { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() const { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() const { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
// bool Process::operator<(Process const& a) const { 
//   return this->cpu_util_ < a.cpu_util_;
// }
 bool Process::operator>(Process const& a) const {
    return cpu_util_ > a.cpu_util_;
  }
