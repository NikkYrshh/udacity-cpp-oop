#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include "linux_parser.h"
#include <iostream>
#include <unistd.h>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line{};
  string key{};
  string value{};
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os{}, kernel{}, version{};
  string line{};
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

std::vector<int> LinuxParser::Pids() {
    std::vector<int> pids;
    for (const auto &entry : std::filesystem::directory_iterator(kProcDirectory)) {
        // Is this a directory?
        if (entry.is_directory()) {
            // Is every character of the name a digit?
            std::string filename = entry.path().filename();
            if (std::all_of(filename.begin(), filename.end(), isdigit)) {
                int pid = std::stoi(filename);
                pids.push_back(pid);
            }
        }
    }
    return pids;
}


float LinuxParser::MemoryUtilization() { 
  float MemTotal{}, MemFree{}; //MemAvail{}, Buffers{};
  string line{}, key{}, value{};
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (getline(stream, line)){
    std::istringstream linestream(line);
    linestream >> key >> value;
    if (key == "MemTotal:")
      {
        MemTotal = std::stof(value);
      }
      else if (key == "MemFree:")
      {
        MemFree = std::stof(value);
      }
//       else if (key == "MemAvailable:"){
//         MemAvail = std::stof(value);
//       }
//        else if (key == "Buffers:"){
//        Buffers = std::stof(value);
//        }
    }
  }
  return (MemTotal - MemFree) / MemTotal;
}

long int LinuxParser::UpTime() { 
  string line{}, time{};
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()){
  getline(stream, line);
  std::istringstream linestream(line);
  linestream >> time;
  }
  
  return std::stol(time);
}

long LinuxParser::Jiffies() { 
  return ActiveJiffies() + IdleJiffies() ; 
}

long LinuxParser::ActiveJiffies(int pid) {
  string line;
  long total{};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        std::string value;
        int count = 1;
        while(linestream >> value) {
            if (count >= 14 && count <= 17) {  
                total += std::stol(value);
            }
            if (count >= 18) {
                break;
            }
            count++;
        }
    }
    return total / sysconf(_SC_CLK_TCK); 
}

// total o active jiffies = kUser_ + kNice_ + kSystem_ + kIRQ_ + kSoftIRQ_ + kSteal_ 
// kGuest_ and  kGuestNice included to user and guest
long LinuxParser::ActiveJiffies() { 
  vector<string>jiffs_arr = CpuUtilization();
  using std::stol;
  return stol(jiffs_arr[kUser_]) + stol(jiffs_arr[kNice_]) + stol(jiffs_arr[kSystem_]) + stol(jiffs_arr[kIRQ_]) + stol(jiffs_arr[kSoftIRQ_]) + stol(jiffs_arr[kSteal_ ]); 
}

long LinuxParser::IdleJiffies() { 
  vector<string>jiffs_arr = CpuUtilization();
  return std::stol(jiffs_arr[kIdle_]) + std::stol(jiffs_arr[kIOwait_]);
}

vector<string> LinuxParser::CpuUtilization() { 
  vector<string>jiffs;
  string line{}, key{}, value{};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key;
    if (key=="cpu"){
      while (linestream >> value)
        jiffs.emplace_back(value);
    }
  } 
  return jiffs; 
}

int LinuxParser::ProcessStatParser(string key_name){
  int target{};
  string line{}, key{}, value{};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (getline(stream, line)){
    std::istringstream linestream(line);
    linestream >> key >> value;
    if (key == key_name){
      target = std::stoi(value);
      break;
    }
   }
  }
  return target;
    }

string LinuxParser::ProcessParser(string target, string file_path){
  string line{}, value{}, key{};
  std::ifstream stream(file_path);
  if (stream.is_open()){
    while (getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if (key == target){
        linestream >> value;
        return value;
      }
    }
  }
    return "Not found";
}

int LinuxParser::TotalProcesses() { 
  return LinuxParser::ProcessStatParser("processes");
    }

int LinuxParser::RunningProcesses() { 
  return LinuxParser::ProcessStatParser("procs_running");  
 }

string LinuxParser::Command(int pid) { 
  string line{}, result{};
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
   getline(stream, line);
   std::istringstream linestream(line);
   return line;
  }
return "None";
}


string LinuxParser::Ram(int pid) { 
  long memory = stoi(ProcessParser("VmSize:", (kProcDirectory + to_string(pid) + kStatusFilename)));
  return to_string(memory / 1000);
}
  
string LinuxParser::Uid(int pid) { 
  return ProcessParser("Uid:", (kProcDirectory + to_string(pid) + kStatusFilename));
}
  

string LinuxParser::User(int pid) { 
  string line{}, value{};
   string uid = Uid(pid);
  std::ifstream stream(kPasswordPath); 
  while (getline(stream, line)){
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    string name, skip, id;
    linestream >> name >> skip >> id;
    if (id == uid){
      return name;
    }
  }
  return "Not found"; 
}

long int LinuxParser::UpTime(int pid) {
  string line{}, value{};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
   getline(stream, line);
   std::istringstream linestream(line);
   for (int i = 0; i < 22; i++ )
     linestream >> line;
  }
  return std::stol(line)/sysconf(_SC_CLK_TCK); 
}
