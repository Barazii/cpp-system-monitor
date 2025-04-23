#include <cstdint>
#include <dirent.h>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

const std::string MemTotal{"MemTotal"};
const std::string MemAvailable{"MemAvailable"};

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
  string os, version, kernel;
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
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float mem{0.0};
  bool memTotalFlag{false};
  bool memAvailableFlag{false};
  std::string line;
  std::string key, value;
  std::map<std::string, std::uint32_t> memory;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stringstream(line);
      stringstream >> key;
      if (key == MemTotal) {
        stringstream >> value;
        memory[MemTotal] = static_cast<std::uint32_t>(std::stoi(value));
        memTotalFlag = true;
      }
      if (key == MemAvailable) {
        stringstream >> value;
        memory[MemAvailable] = static_cast<std::uint32_t>(std::stoi(value));
        memAvailableFlag = true;
      }
      if (memTotalFlag && memAvailableFlag) {
        mem = static_cast<float>(memory[MemTotal] - memory[MemAvailable]) /
              static_cast<float>(memory[MemTotal]);
        return mem;
      }
    }
  }
  return mem;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  uint64_t uptime;
  std::string line, value;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  uptime = static_cast<uint64_t>(std::stoul(value));
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<string> jiffies = CpuUtilization();
  long t_jiffies = 0;
  for (string jiffie : jiffies) {
    t_jiffies += std::stoi(jiffie);
  }
  return t_jiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) {
  long a_jiffies = 0;
  string utime;
  string stime;
  string line;
  string skip;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 1; i < 14; ++i) {
      linestream >> skip;
    }
    linestream >> utime >> stime;
  }
  a_jiffies = std::atol(utime.c_str()) + std::atol(stime.c_str());
  return a_jiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  long a_jiffies = 0;
  a_jiffies = Jiffies() - IdleJiffies();
  return a_jiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> jiffies = CpuUtilization();
  long i_jiffies = 0;
  long idle = std::stoi(jiffies[3]);
  long iowait = std::stoi(jiffies[4]);
  i_jiffies = idle + iowait;
  return i_jiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> timers;
  string timer;
  string line;
  string skip;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> skip;
    for (int i = 0; i < 10; ++i) {
      linestream >> timer;
      timers.push_back(timer);
    }
  }
  return timers;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  uint16_t count{0};
  std::filesystem::directory_iterator path_iterator(kProcDirectory);
  for (std::filesystem::directory_entry entry : path_iterator) {
    std::string filename = entry.path().filename();
    if (std::all_of(filename.begin(), filename.end(),
                    [](char c) { return std::isdigit(c); })) {
      count++;
    }
  }
  return count;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  uint16_t count{0};
  std::string line, key, value;
  std::filesystem::directory_iterator path_iterator(kProcDirectory);
  for (std::filesystem::directory_entry entry : path_iterator) {
    std::string filename = entry.path().filename();
    if (std::all_of(filename.begin(), filename.end(),
                    [](char c) { return std::isdigit(c); })) {
      std::ifstream filestream(kProcDirectory + filename + "/" + "status");
      if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
          std::replace(line.begin(), line.end(), ':', ' ');
          std::istringstream linestream(line);
          linestream >> key;
          if (key == "State") {
            linestream >> value;
            if (value == "R")
              count++;
            break;
          }
        }
      }
    }
  }
  return count;
}

std::vector<int> LinuxParser::getProcessesPids() {
  std::vector<int> pids{};
  std::filesystem::directory_iterator path_iterator(kProcDirectory);
  for (std::filesystem::directory_entry entry : path_iterator) {
    std::string filename = entry.path().filename();
    if (std::all_of(filename.begin(), filename.end(),
                    [](char c) { return std::isdigit(c); })) {
      pids.push_back(std::stoi(filename));
    }
  }
  return pids;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::string line;
  std::ifstream filestream{kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename};
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) {
  std::string line, key, ram{""};
  std::ifstream filestream{kProcDirectory + std::to_string(pid) +
                           kStatusFilename};
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream{line};
      linestream >> key;
      key.pop_back();
      if (key == "VmSize") {
        linestream >> ram;
        return ram;
      }
    }
  }
  return ram;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) {
  long ticks = 0;
  string line;
  string skip;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 1; i < 22; ++i) {
      linestream >> skip;
    }
    linestream >> ticks;
  }
  return ticks / sysconf(_SC_CLK_TCK);
}

float LinuxParser::CpuUtilization(int pid) {
  float a_process = static_cast<float>(LinuxParser::ActiveJiffies(pid));
  float a_processor = static_cast<float>(LinuxParser::ActiveJiffies());
  return a_process / a_processor;
}