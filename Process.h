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

    Process(std::string id, int arrival_time, ProcessType type);
    bool is_cpu_bound() const;
};

#endif // PROCESS_H