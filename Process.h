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
    std::vector<int> original_cpu_bursts;
    std::vector<int> io_bursts;
    std::vector<bool> done_in_1slice;
    double tau; // Estimated CPU burst time

    // New member variables
    int wait_time;
    int turnaround_time;
    int context_switches;
    int preemptions;
    int total_cpu_time;
    int total_time;
    int initial_wait_time = 0;
    int end_wait_time;

    Process(std::string id, int arrival_time, ProcessType type, double initial_tau);
    bool is_cpu_bound() const;
    void sliced(int time, int slice_time, int original_burst);

};

#endif // PROCESS_H