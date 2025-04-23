#include <cstddef>
#include <set>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor &System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process> &System::Processes() {
  std::vector<int> processesPids = LinuxParser::getProcessesPids();
  processes_.clear();
  for (int pid : processesPids) {
    Process process;
    process.setPid(pid);
    process.setCommand(LinuxParser::Command(pid));
    process.setCpuUtilization(LinuxParser::CpuUtilization(pid));
    process.setRam(LinuxParser::Ram(pid));
    // process.setUpTime(LinuxParser::UpTime(pid));
    processes_.push_back(process);
  }
  std::sort(processes_.begin(), processes_.end(), std::greater<Process>());
  return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }