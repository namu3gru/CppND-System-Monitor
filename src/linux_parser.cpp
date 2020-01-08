#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "linux_parser.h"

using std::atoi;  // convert string to int
using std::atol;  // convert to long
using std::stof;  // convert to float
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
  string os, kernel, temp;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    
    linestream >> os >> temp >> kernel;
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
  string memTotal,memFree, memAvailable, buffers, title, line;
  std::ifstream strem(kProcDirectory + kMeminfoFilename);
  if(strem.is_open())
  {
    std::getline(strem, line);
    std::istringstream linestream(line);
    linestream >> title >> memTotal;

    std::getline(strem, line);
    std::istringstream linestreammemFree(line);
    linestreammemFree >> title >> memFree;

    std::getline(strem, line);
    std::istringstream linestreammemAvailable(line);
    linestreammemAvailable >> title >> memAvailable;

    std::getline(strem, line);
    std::istringstream linestreambuffers(line);
    linestreambuffers >> title >> buffers;

  }
  float memUtil = (stof(memTotal.c_str()) - stof(memFree.c_str())) / stof(memTotal.c_str());
  return memUtil;

 }

// TODO: Read and return the system uptime
long LinuxParser::SysUpTime() { 
  string title, upTime, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime;
  }
  return atol(upTime.c_str());
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  vector<string> jiffies = CpuUtilization();
  long totalJiffiesSinceBoot = 0;
  for(string s:jiffies)
  {
    totalJiffiesSinceBoot += atol(s.c_str());
  }
  return totalJiffiesSinceBoot;
 }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  pid = 0;
  return pid; 
  }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return LinuxParser::Jiffies() - LinuxParser::IdleJiffies();}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> jiffies = CpuUtilization();
  long idleJiffiesSinceBoot = atol(jiffies[3].c_str());
  return idleJiffiesSinceBoot;
 }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> v{};
  string user, nice, system, idle, iowait, irq, softirq, steal;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

 }
 v.push_back(user);
 v.push_back(nice);
 v.push_back(system);
 v.push_back(idle);
 v.push_back(iowait);
 v.push_back(irq);
 v.push_back(softirq);
 v.push_back(steal);
 return v;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string name = "processes";
  string title, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  while (std::getline(stream, line))
  {
    if(line.compare(0, name.size(), name) == 0)
    {
      std::istringstream linestream(line);
      linestream >> title >> value;
      break;
    }
  }
  return atoi(value.c_str());
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string name = "procs_running";
  string title, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  while(std::getline(stream, line))
  {
    if(line.compare(0, name.size(), name) == 0)
    {
      std::istringstream linestream(line);
      linestream >> title >> value;
      break;
    }
  }
  return atoi(value.c_str());
 }

float LinuxParser::CpuUtilization(int pid) {
  string line;
  string value;
  float result;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  getline(stream, line);
  string str = line;
  std::istringstream buf(str);
  std::istream_iterator<string> beg(buf), end;
  vector<string> values(beg, end);  // done!

  float utime = UpTime(pid);
  float stime = stof(values[14]);
  // float cutime = stof(values[15]); // don't use now
  // float cstime = stof(values[16]); // don't use now
  float starttime = stof(values[21]);
  float uptime = SysUpTime();  // sys uptime
  float freq = sysconf(_SC_CLK_TCK);
  float total_time = utime + stime;
  float seconds = uptime - (starttime / freq);
  result = 100.0 * ((total_time / freq) / seconds);
  return result;
}
// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string cmd;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, cmd);
  }
  return cmd;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string name = "VmSize";
  string title, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line;
  while (std::getline(stream, line)) {
    if (line.compare(0, name.size(), name) == 0) {
      std::istringstream linestream(line);
      linestream >> title >> value;
      value = to_string((stof(value.c_str()) / 1000));
      return value.substr(0, 6);
    }
  }
  return "0";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {   
  static string name = "Uid";
  string title, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  static string line;
  while (std::getline(stream, line)) {
    if (line.compare(0, name.size(), name) == 0) {
      std::istringstream linestream(line);
      linestream >> title >> value;
      break;
    }
  }
  return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
    string name = "x:" + Uid(pid);
  string title, value;
  std::ifstream stream(kPasswordPath);
  string line;
  while (std::getline(stream, line)) {
    if (line.find(name) != std::string::npos) {
      value = line.substr(0, line.find(":"));
    }
  }
  return value;
 }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long int LinuxParser::UpTime(int pid) {
  string line;
  string value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  std::getline(stream, line);
  std::istringstream linestream(line);
  std::istream_iterator<string> beg(linestream), end;
  vector<string> values(beg, end);

  long int seconds = float(stof(values[13]) / sysconf(_SC_CLK_TCK));
  return seconds;
 }
