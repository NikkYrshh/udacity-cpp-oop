#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ConvertTime(const long time);
  std::string ElapsedTime(const long times);  // TODO: See src/format.cpp
};                                    // namespace Format

#endif