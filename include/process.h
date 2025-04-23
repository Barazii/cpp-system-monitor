#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
public:
  int Pid();                              // TODO: See src/process.cpp
  std::string User();                     // TODO: See src/process.cpp
  std::string Command();                  // TODO: See src/process.cpp
  float CpuUtilization();                 // TODO: See src/process.cpp
  std::string Ram();                      // TODO: See src/process.cpp
  long int UpTime();                      // TODO: See src/process.cpp
  bool operator<(Process const &a) const; // TODO: See src/process.cpp
  bool operator>(Process const &a) const; // TODO: See src/process.cpp

  // setters
  void setCommand(std::string command);
  void setPid(int pid);
  void setCpuUtilization(float cpuUtilization);
  void setRam(std::string ram);
  void setUpTime(long uptime);

  // Declare any necessary private members
private:
  int pid_{0};
  std::string user_{""};
  std::string command_{""};
  float cupUtilization_{0.0};
  std::string ram_{""};
  long uptime_{0};
};

#endif