// Process.cpp
#include "Process.h"

Process::Process(std::string id, int arrival_time, ProcessType type, double initial_tau)
    : id(std::move(id)), state(READY), type(type), arrival_time(arrival_time), tau(initial_tau),
      wait_time(0), turnaround_time(0), context_switches(0), preemptions(0), total_cpu_time(0), total_time(0) {}

bool Process::is_cpu_bound() const {
    return type == CPU_BOUND;
}