// Process.cpp
#include "Process.h"

Process::Process(std::string id, int arrival_time, ProcessType type, double initial_tau)
    : id(std::move(id)), state(READY), type(type), arrival_time(arrival_time), tau(initial_tau),
      wait_time(0), turnaround_time(0), context_switches(0), preemptions(0), total_cpu_time(0), total_time(0) {}

bool Process::is_cpu_bound() const {
    return type == CPU_BOUND;
}


void Process::sliced(int time, int slice_time, int original_burst) {
    //find the index of the original burst
    int index = 0;
    for (size_t i = 0; i < original_cpu_bursts.size(); i++) {
        if (original_cpu_bursts[i] == original_burst) {
            index = i;
            break;
        }
    }
    if(original_burst > slice_time) {
        done_in_1slice[index] = false;
    } else {
        done_in_1slice[index] = true;
    }
}

