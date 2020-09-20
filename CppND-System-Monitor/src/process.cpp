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
  return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const{
  string line;
  string random;
  long starttime;
  long uptime = LinuxParser::UpTime();
  long utime, stime, cutime, cstime, totaltime, seconds;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid_) + LinuxParser::kStatFilename);
  if (stream.is_open()) {
  std::getline (stream, line);
  std::istringstream linestream(line);
  for (int i = 0; i <= 21; i++) {
  	if (i == 13) {
    	linestream >> utime;
    }
    if (i == 14) {
      linestream >> stime;
    }
    if (i == 15) {
      linestream >> cutime;
    }
    if (i == 16) {
      linestream >> cstime;
    }
    if (i == 21) {
    	linestream >> starttime;
    }
    else {
      linestream >> random;
    }
  }
  totaltime = utime + stime + cutime + cstime;
  seconds = uptime - LinuxParser::UpTime(pid_);
  return (float(totaltime) / sysconf(_SC_CLK_TCK)) / seconds;
  }
}

// TODO: Return the command that generated this process
string Process::Command() {
 return LinuxParser::Command(pid_);
}

// TODO: Return this process's memory utilization
string Process::Ram() {
  return LinuxParser::Ram(pid_);
}

// TODO: Return the user (name) that generated this process
string Process::User() {
  return LinuxParser::User(pid_);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  return LinuxParser::UpTime(pid_);

}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const&a)  const{
  return (this->CpuUtilization() > a.CpuUtilization());
}