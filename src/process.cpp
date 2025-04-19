#include <cctype>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

void Process::Pid(int pid)
{
  pid_ = pid;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_utilization_; }

void Process::CpuUtilization(int pid)
{
  float a_process = static_cast<float>(LinuxParser::ActiveJiffies(pid));
  float a_processor = static_cast<float>(LinuxParser::ActiveJiffies());
  cpu_utilization_ = a_process / a_processor;
}

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

void Process::Command(int pid)
{
  command_ = LinuxParser::Command(pid);
}

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

void Process::Ram(int pid)
{
  int kbytes = std::atoi(LinuxParser::Ram(pid).c_str());
  int mbytes = kbytes / 1000; // in MB
  ram_ = std::to_string(mbytes);
}

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

void Process::User(int pid)
{
  user_ = LinuxParser::User(pid);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return up_time_; }

void Process::UpTime(int pid)
{
  up_time_ = LinuxParser::UpTime(pid);
}

// TODO: Overload the "less than" comparison operator for Process objects
// Overloads the less operator according to cpu utilization
bool Process::operator<(Process const &a) const
{
  return this->cpu_utilization_ < a.cpu_utilization_;
}

// Overloads the greater operator according to cpu utilization
bool Process::operator>(Process const &a) const
{
  return this->cpu_utilization_ > a.cpu_utilization_;
}