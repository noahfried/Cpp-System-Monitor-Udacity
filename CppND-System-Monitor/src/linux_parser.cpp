#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> version >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  string line;
  string name;
  float value, total, free;
  string memtotal = "MemTotal:";
  string memfree = "MemFree:";
  if (stream.is_open()) {
    while (std::getline(stream, line)){;
    std::istringstream stream(line);
    while (stream >> name >> value) {
      if (name.compare(memtotal) == 0) {
        total = value;
      }
      if (name.compare(memfree) == 0) {
        free = value;
        return ((total - free) / total);
      }
    }
  }
  }
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime; 
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream stream(line);
    stream >> uptime;
    return uptime;
    }
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return (UpTime() * sysconf(_SC_CLK_TCK));
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
//   string line;
//   std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
//   if (stream.is_open()) {
    
//     for (int i = 0; i <= 16; i++) {
//       std::istringstream stream(line)
//       if (i == 13) {
        
  
//   return 0;
// }
//     }
//   }
}
// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization() {
  std::string cpu;
  std::string line;
  float user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice, totalidle, totalsystem, virtalltime, total;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    stream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
  user = user - guest;
  nice = nice - guest_nice;
  totalidle =  idle + iowait;
  totalsystem = system + irq + softirq;
  virtalltime = guest + guest_nice;
  total = user + nice + totalsystem + totalidle + steal + virtalltime;
  return (total - totalidle) / idle;
    
}
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, name;
  int a;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
   while (std::getline(stream, line)){
    std::istringstream stream(line);
     while (stream >> name >> a) {
      if (name == "processes") {
        return a;
      }
     }
   }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, name;
  int a;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream stream(line);
      while (stream >> name >> a) {
        if (name == "procs_running") {
          return a;
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
  std::getline(stream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line, a, b;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream stream(line);
      while (stream >> a >> b) {
        if (a == "VmSize:") {
          return b;
    }
      }
    }
  }
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line, a, b;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream stream(line);
      stream >> a >> b; 
        if (a == "Uid:") {
          return b;
    }
      
    }
  }
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line, key, space, value;
  string id = Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
    std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> space >> value){
        if (value == id){
        return key;
        }
  }
  }
  return string();
}
}
// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, random;
  long uptime;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
  std::getline(stream, line);
  std::istringstream linestream(line);
    for(int i = 0; i <= 21; i++) {
    if (i == 21) {
      linestream >> uptime;
      return uptime / sysconf(_SC_CLK_TCK);
    }
    else {
      linestream >> random;
  }
}
  }
}