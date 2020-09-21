#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long min = seconds / 60;
  seconds = seconds % 60;
  long hours = min / 60;
  min = min % 60;
  string h = std::to_string(hours);
  string m = std::to_string(min);
  string s = std::to_string(seconds);
	h.insert(0, 2 - h.length(), '0');
  m.insert(0, 2 - m.length(), '0');
  s.insert(0, 2 - s.length(), '0');

  return (h + ":" + m + ":" + s);
}