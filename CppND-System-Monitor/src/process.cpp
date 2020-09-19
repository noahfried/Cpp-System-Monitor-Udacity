#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() {
  return pid;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const{
  string line;
  string random;
  long starttime = LinuxParser::UpTime(pid);
  long uptime = LinuxParser::UpTime();
  long utime, stime, cutime, cstime, totaltime, seconds;
  long Hertz = sysconf(_SC_CLK_TCK);
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename);
  if (stream.is_open()) {
  std::getline (stream, line);
  std::istringstream linestream(line);
  for (int i = 0; i <= 16; i++) {
  	if (i == 13) {
    	linestream >> utime;
    }
    else if (i == 14) {
      linestream >> stime;
    }
    else if (i == 15) {
      linestream >> cutime;
    }
    else if (i == 16) {
      linestream >> cstime;
    }
    else {
      linestream >> random;
    }
  }
  totaltime = utime + stime;
  totaltime = totaltime + cutime + cstime;
  seconds = uptime - starttime;
  return (100 * ((totaltime / Hertz) / seconds));
  }
}

// TODO: Return the command that generated this process
string Process::Command() {
  string line;
  string command;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kCmdlineFilename);
  if (stream.is_open()) {
  std::getline(stream, line);
    std::istringstream linestream(line);
    stream >> command;
    return command;
  }
}

// TODO: Return this process's memory utilization
string Process::Ram() {
  return LinuxParser::Ram(pid);
}

// TODO: Return the user (name) that generated this process
string Process::User() {
  return LinuxParser::User(pid);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  return LinuxParser::UpTime(pid);

}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const&a)  const{
  return (this->CpuUtilization() < a.CpuUtilization());
}