#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <unordered_set>
#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
    set<int> no_repeat;
    for (Process process : processes_) {
        no_repeat.insert(process.Pid());
    }
    std::vector<int> pids = LinuxParser::Pids();
    for (int pid: pids) {
        if (no_repeat.find(pid) == no_repeat.end()) {
            processes_.emplace_back(Process(pid));
    }
}for (Process& process: processes_) {
        process.CpuUtilization();
  }
    std::sort(processes_.begin(), processes_.end(), std::greater<Process>());

    return processes_; 
}

std::string System::Kernel() const { return LinuxParser::Kernel(); }


float System::MemoryUtilization() const { return LinuxParser::MemoryUtilization(); }


std::string System::OperatingSystem() const { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() const { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() const { return LinuxParser::TotalProcesses(); }

long int System::UpTime() const { return LinuxParser::UpTime(); }
