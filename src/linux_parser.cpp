#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <time.h>

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
    linestream >> os >> version >> kernel;
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  double value;

  std::unordered_map<std::string, double> meminfo;

  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');

      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        meminfo[key] = value;
      }
    }
  }
  // Memory utilization calculation:
  // * Total used memory = MemTotal - MemFree
  // * Non cache/buffer memory (green) = Total used memory - (Buffers + Cached memory)
  // * Buffers (blue) = Buffers
  // * Cached memory (yellow) = Cached + SReclaimable - Shmem
  // * Swap = SwapTotal - SwapFree
  // ref:https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290

  double totalUsedMem = meminfo["MemTotal"] - meminfo["MemFree"];
  // double nonBuffCachMem /*green*/ = totalUsedMem - (meminfo["Buffers"]/*blue*/ + meminfo["Cached"]);
  // double cachMem /*yellow*/ = meminfo["Cached"] + meminfo["SReclaimable"] - meminfo["Shmem"];
  // double swap = meminfo["SwapTotal"] - meminfo["SwapFree"];

  return float(totalUsedMem/meminfo["MemTotal"]); 
}

// DONE: Read and return the system uptime
long int LinuxParser::UpTime() {
  std::string uptimestr;
  std::string idletimestr;

  string line;
  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> uptimestr >> idletimestr;
    }
  }

  return std::stol(uptimestr);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line, value;

  vector<string> pidStats = {};
  
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        pidStats.push_back(value);
      }
    }
  }
  if(pidStats.size() > 22){
    long pidTotalTime = std::stol(pidStats[13])  //utime
                      + std::stol(pidStats[14])  //stime
                      + std::stol(pidStats[15])  //cutime
                      + std::stol(pidStats[16]); //cstime

    return pidTotalTime;
  }
  return 0;


}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// DONE: Read and return CPU utilization
std::vector<long> LinuxParser::CpuUtilization() {

  string line;
  string value;

  std::vector<long> cputil; //possible
  std::ifstream filestream(kProcDirectory+kStatFilename);
  
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while(linestream >> value){
      if(value != "cpu"){
        cputil.push_back(stol(value));
        value = "";
      }
    } 
  }
  return cputil;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

// DONE: Read and return the command associated with a process
// /proc/[pid]/cmdline
string LinuxParser::Command(int pid) {
  string line, value;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> value;
      return value;
    }
  }
  return "";
}

// DONE: Read and return the memory used by a process 'in kb'
// /proc/[pid]/status.
string LinuxParser::Ram(int pid) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        return to_string((std::stol(value))/1000);
      }
      
    }
  }
  return "";
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return "";
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line, key, value, password;

  string uid = LinuxParser::Uid(pid);
  
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> password >> key) {
        if (key == uid) {
          return value;
        }
        value="";
      }
    }
  }
  return "";
}

// DONE: Read and return the uptime of a process 'in seconds'
// found at /proc/[PID]/stat
long LinuxParser::UpTime(int pid) {
  string line, value;

  vector<string> pidStats = {};
  
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        pidStats.push_back(value);
      }
    }
  }
  if(pidStats.size() > 22){
    long sysUpTime = LinuxParser::UpTime();
    long procStartTime = std::stol(pidStats[21]); /*start time in jiffies*/ 
    return long(sysUpTime - (procStartTime/sysconf(_SC_CLK_TCK)));
  }
  return 0;
}