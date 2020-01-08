#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  Processor();
  long prevIdle;
  long prevNonIdle;
  long prevTotal;
  // TODO: Declare any necessary private members
 private:
};

#endif