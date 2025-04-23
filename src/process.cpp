#include <cctype>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return cupUtilization_; }

// Return the command that generated this process
string Process::Command() { return command_; }

// Return this process's memory utilization
string Process::Ram() { return ram_; }

// Return the user (name) that generated this process
string Process::User() { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const &a) const {
  return this->cupUtilization_ < a.cupUtilization_;
}

// Overloads the greater operator according to cpu utilization
bool Process::operator>(Process const &a) const {
  return this->cupUtilization_ > a.cupUtilization_;
}

void Process::setCommand(std::string command) { command_ = command; }
void Process::setPid(int pid) { pid_ = pid; }
void Process::setCpuUtilization(float cpuUtilization) {
  cupUtilization_ = cpuUtilization;
}
void Process::setRam(std::string ram) { ram_ = ram; }
void Process::setUpTime(long uptime) { uptime_ = uptime; }