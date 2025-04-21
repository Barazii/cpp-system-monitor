#include <dirent.h>
#include <string>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <filesystem>
#include <map>
#include <iterator>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

const string MemTotal{"MemTotal"};
const string MemFree{"MemFree"};

// Parses the file to find a value for a given key
string LinuxParser::KeyValParser(string key, string path)
{
  string value = "n/a";
  string line;
  string temp;
  std::ifstream stream(path);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> temp;
      if (temp == key)
      {
        linestream >> value;
        return value;
      }
    }
    throw std::runtime_error("EOF occurs and key not found");
  }
  throw std::runtime_error("File not found");
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem()
{
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "PRETTY_NAME")
        {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel()
{
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
// 1. open dir
// 2. read each file in the dir
// 3. check if the file is a directory
// 4. check if the file name is a number
// 5. convert the file name to an int
// 6. push the int to the vector
vector<int> LinuxParser::Pids()
{
  vector<int> pids;
  const std::filesystem::path procDir{kProcDirectory};
  for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator(procDir))
  {
    if (entry.is_directory())
    {
      string filename = entry.path().filename().string();
      if (std::all_of(filename.begin(), filename.end(), isdigit))
      {
        pids.push_back(std::stoi(filename));
      }
    }
  }
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
{
  string key;
  string skip;
  string value;
  string line;
  std::map<string, float> memory;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open())
  {
    for (int i = 0; i < 10; ++i)
    {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key >> value >> skip;
      if (!key.empty())
      {
        key.pop_back();
      }
      if (key == MemTotal || key == MemFree)
      {
        memory[key] = std::stof(value);
      }
    }
  }
  return (memory[MemTotal] - memory[MemFree]) / memory[MemTotal];
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime()
{
  long uptime = 0.0;
  string temp = "0.0";
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> temp;
  }
  uptime = std::stoi(temp);
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies()
{
  vector<string> jiffies = CpuUtilization();
  long t_jiffies = 0;
  for (string jiffie : jiffies)
  {
    t_jiffies += std::stoi(jiffie);
  }
  return t_jiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid)
{
  long a_jiffies = 0;
  string usertime;
  string systemtime;
  string line;
  string skip;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open())
  {
    std::cout << "This is one time: " << std::endl;
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 1; i < 14; ++i)
    {
      linestream >> skip;
    }
    linestream >> usertime >> systemtime;
  }
  a_jiffies = std::stol(usertime) + std::stol(systemtime);
  return a_jiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{
  long a_jiffies = 0;
  a_jiffies = Jiffies() - IdleJiffies();
  return a_jiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies()
{
  vector<string> jiffies = CpuUtilization();
  long i_jiffies = 0;
  long idle = std::stoi(jiffies[3]);
  long iowait = std::stoi(jiffies[4]);
  i_jiffies = idle + iowait;
  return i_jiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization()
{
  vector<string> timers;
  string timer;
  string line;
  string skip;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> skip;
    for (std::istream_iterator<string> it(linestream); it != std::istream_iterator<string>(); ++it)
    {
      timers.push_back(*it);
    }
  }
  return timers;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
  int t_processes = 0;
  string path = kProcDirectory + kStatFilename;
  string result = LinuxParser::KeyValParser("processes", path);
  t_processes = std::stoi(result);
  return t_processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses()
{
  int a_processes = 0;
  string path = kProcDirectory + kStatFilename;
  string result = LinuxParser::KeyValParser("procs_running", path);
  a_processes = std::stoi(result);
  return a_processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid)
{
  string line = "n/a";
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid)
{
  string path = kProcDirectory + "/" + std::to_string(pid) + kStatusFilename;
  return LinuxParser::KeyValParser("VmSize:", path);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
#include <iostream>

string LinuxParser::Uid(int pid)
{
  string line;
  string key;
  string value = "";
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "Uid")
        {
          return value;
        }
      }
    }
  }
  return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid)
{
  // read the user ID for this process
  string uid = Uid(pid);
  string line;
  string key;
  string value = "";
  string other;
  // find user name for this user ID in /etc/passwd
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> other >> key)
      {
        if (key == uid)
        {
          return value;
        }
      }
    }
  }
  return value;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid)
{
  long ticks = 0;
  string line;
  string skip;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 1; i < 22; ++i)
    {
      linestream >> skip;
    }
    linestream >> ticks;
  }
  return ticks / sysconf(_SC_CLK_TCK);
}
