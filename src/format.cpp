#include <string>

#include "format.h"

using std::string;

string Format::ConvertTime(const long time){
  string time_str {};
  if(time < 10){
    return "0" + std::to_string(time);
  
  }else {
    return std::to_string(time);
  }
}

string Format::ElapsedTime(const long seconds) { 
  long sec = seconds % 60;
  long min = (seconds / 60) % 60;
  long hr = (seconds / 3600) % 24; 
  // %24 may be ommited then the system will show all the time (in my case it was HHH)
  
  return string(ConvertTime(hr) + ":" + ConvertTime(min) + ":" + ConvertTime(sec)); 
}