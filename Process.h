// Process.h
#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

enum ProcessState { RUNNING, READY, WAITING };
enum ProcessType { CPU_BOUND, IO_BOUND };

class Process {
public:
    std::string id;
    ProcessState state;
    ProcessType type;
    int arrival_time;
    std::vector<int> cpu_bursts;
    std::vector<int> io_bursts;
    double tau; // Estimated CPU burst time

    // New member variables
    int wait_time;
    int turnaround_time;
    int context_switches;
    int preemptions;
    int total_cpu_time;
    int total_time;

    Process(std::string id, int arrival_time, ProcessType type, double initial_tau);
    bool is_cpu_bound() const;
};

#endif // PROCESS_H