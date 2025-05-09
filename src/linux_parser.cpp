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
        memory[MemTotal] = std::stoi(value);
        memTotalFlag = true;
      }
      if (key == MemAvailable) {
        stringstream >> value;
        memory[MemAvailable] = std::stoi(value);
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

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<string> cpu_times = CpuUtilization();
  long total = 0;
  for (const string &time : cpu_times) {
    total += stol(time);
  }
  return total;
}

// TODO: Read and return the number of active jiffies for a PID
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

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> cpu_times = CpuUtilization();
  return stol(cpu_times[CPUStates::kUser_]) +
         stol(cpu_times[CPUStates::kNice_]) +
         stol(cpu_times[CPUStates::kSystem_]) +
         stol(cpu_times[CPUStates::kIRQ_]) +
         stol(cpu_times[CPUStates::kSoftIRQ_]) +
         stol(cpu_times[CPUStates::kSteal_]);
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> cpu_times = CpuUtilization();
  return stol(cpu_times[CPUStates::kIdle_]) +
         stol(cpu_times[CPUStates::kIOwait_]);
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> timers;
  string timer;
  string line;
  string skip;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> skip; // Skip "cpu" prefix
    while (linestream >> timer) {
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

// Read and return the uptime of a process
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