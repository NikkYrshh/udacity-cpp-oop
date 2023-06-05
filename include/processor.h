#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
 Processor();
  float Utilization();  

 private:
 long prev_idle_;
 long prev_active_;
 long prev_total_;
};

#endif