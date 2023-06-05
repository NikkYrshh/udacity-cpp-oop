#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  int Pid() const;                               
  std::string User() const;                      
  std::string Command() const;                  
  float CpuUtilization();                  
  std::string Ram() const;                       
  long int UpTime() const;                       
  bool operator>(Process const& a) const;  
  Process(int pid);
//   void Update();
    

 private:
  int pid_;
  std::string user_;
  std::string command_;
  std::string ram_;
  long int uptime_;
  float cpu_util_;

};

#endif