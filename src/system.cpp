#include <cstddef>
#include <mutex>
#include <set>
#include <string>
#include <thread>
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

// Return the system's CPU
Processor &System::Cpu() { return cpu_; }

// Helper function to create a Process object in a thread
void createProcess(int pid, Process &process)
{
  process.setPid(pid);
  process.setCommand(LinuxParser::Command(pid));
  process.setCpuUtilization(LinuxParser::CpuUtilization(pid));
  process.setRam(LinuxParser::Ram(pid));
  process.setUpTime(LinuxParser::UpTime(pid));
}

// Return a container composed of the system's processes using multi-threading
vector<Process> &System::Processes()
{
  std::vector<int> processesPids = LinuxParser::getProcessesPids();
  processes_.clear();

  // Pre-allocate space for processes and threads
  std::vector<Process> tempProcesses(processesPids.size());
  std::vector<std::thread> threads;
  threads.reserve(processesPids.size());

  // Create threads for each PID
  for (size_t i = 0; i < processesPids.size(); ++i)
  {
    threads.emplace_back(createProcess, processesPids[i],
                         std::ref(tempProcesses[i]));
  }

  // Wait for all threads to complete
  for (std::thread &thread : threads)
  {
    thread.join();
  }

  // Move processes to the member vector
  processes_ = std::move(tempProcesses);

  // Sort processes by CPU utilization
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