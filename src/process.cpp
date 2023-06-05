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


int Process::Pid() const { return pid_; }

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

string Process::Command() const { return command_; }

string Process::Ram() const { return ram_; }

string Process::User() const { return user_; }

long int Process::UpTime() const { return uptime_; }


 bool Process::operator>(Process const& a) const {
    return cpu_util_ > a.cpu_util_;
  }
