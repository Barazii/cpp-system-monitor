#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
public:
  float Utilization();

private:
  long prevTotal_{0};
  long prevIdle_{0};
};

#endif