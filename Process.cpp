#include "Process.h"

Process::Process(std::string id, int arrival_time, ProcessType type)
    : id(std::move(id)), state(READY), type(type), arrival_time(arrival_time) {}

bool Process::is_cpu_bound() const {
    return type == CPU_BOUND;
}