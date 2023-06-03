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
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    auto pid_arr = LinuxParser::Pids();

  std::unordered_set<int> curr_pids(pid_arr.begin(), pid_arr.end());

    for (auto iter = processes_.begin(); iter != processes_.end();) {
        if (curr_pids.count(iter->Pid())) {
            iter->Update();
            ++iter;
        } else {
            iter = processes_.erase(iter);
        }
    }

    for (const auto& pid : pid_arr) {
        bool found = false;
        for (const auto& proc : processes_) {
            if (proc.Pid() == pid) {
                found = true;
                break;
            }
        }
        if (!found) {
            Process process(pid);
            processes_.emplace_back(process);
        }
    }

    std::sort(processes_.begin(), processes_.end());

    return processes_;
}



// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() const { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() const { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() const { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() const { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() const { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() const { return LinuxParser::UpTime(); }
