#include <iostream>
#include <queue>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <fstream>
#include <unordered_set>
#include <algorithm>
#include "Process.h"

using namespace std;

queue<Process*> ready_queue;
int bound = 0;
int tcs = 0;
double alpha = 0.0;
int tslice = 0;

double next_exp(const double lambda) {
    const double r = drand48();
    return -log(r) / lambda;
}

void generate_bursts(Process& process, const double lambda) {
    const int num_cpu_bursts = ceil(drand48() * 32);
    for (int j = 0; j < num_cpu_bursts; ++j) {
        int cpu_burst;
        int io_burst;
        do {
            cpu_burst = ceil(next_exp(lambda));
        } while (cpu_burst > bound);
        if (j < num_cpu_bursts - 1) {
            do {
                io_burst = ceil(next_exp(lambda));
            } while (io_burst > bound);
        }
        if (process.is_cpu_bound()) {
            cpu_burst *= 4;
        } else {
            io_burst *= 8;
        }

        process.cpu_bursts.push_back(cpu_burst);
        if (j < num_cpu_bursts - 1) {
            process.io_bursts.push_back(io_burst);
        }
    }
}

void initialize_processes(vector<Process>& processes, const int num_processes, const int num_cpu_bound, const double lambda) {
    char id_letter = 'A';
    int id_number = 0;
    double initial_tau = ceil(1.0 / lambda);

    for (int i = 0; i < num_processes; ++i) {
        const string id = string(1, id_letter) + to_string(id_number);

        double r;
        do {
            r = next_exp(lambda);
        } while (r > bound);
        const int arrival_time = floor(r);

        const ProcessType type = (i < num_cpu_bound) ? CPU_BOUND : IO_BOUND;
        Process process(id, arrival_time, type, initial_tau);

        generate_bursts(process, lambda);

        processes.push_back(process);

        id_number++;
        if (id_number > 9) {
            id_number = 0;
            id_letter++;
        }
    }
}

double calculate_cpu_utilization(const vector<Process>& processes) {
    int total_cpu_time = 0;
    int total_time = 0;

    for (const Process& process : processes) {
        total_cpu_time += process.total_cpu_time;
        total_time += process.total_time;
    }

    return (total_time > 0) ? static_cast<double>(total_cpu_time) / total_time : 0;
}

void write_algorithm_to_file(const string& algorithm, ofstream &outfile, const vector<Process>& processes) {
    outfile << ("\noutput stats here");
}

void calculate_and_write_statistics(const vector<Process>& processes, ofstream &outfile) {

    int num_cpu_bound = 0;
    int num_io_bound = 0;
    int total_cpu_bound_cpu_burst_time = 0;
    int total_cpu_bound_io_burst_time = 0;
    int total_cpu_burst_time = 0;
    int total_io_bound_cpu_burst_time = 0;
    int total_io_bound_io_burst_time = 0;
    int total_io_burst_time = 0;
    int total_cpu_bursts = 0;
    int total_io_bursts = 0;
    int total_cpu_bound_cpu_bursts = 0;
    int total_io_bound_cpu_bursts = 0;
    int total_cpu_bound_io_bursts = 0;
    int total_io_bound_io_bursts = 0;

    for(const Process& process : processes) {
        if (process.is_cpu_bound()) {
            num_cpu_bound++;
            total_cpu_bound_cpu_bursts += static_cast<int>(process.cpu_bursts.size());
            total_cpu_bound_io_bursts += static_cast<int>(process.io_bursts.size());
            for (const int cpu_burst : process.cpu_bursts) {
                total_cpu_burst_time += cpu_burst;
                total_cpu_bound_cpu_burst_time += cpu_burst;
                total_cpu_bursts++;
            }
            for (const int io_burst : process.io_bursts) {
                total_io_burst_time += io_burst;
                total_cpu_bound_io_burst_time += io_burst;
                total_io_bursts++;
            }
        } else {
            num_io_bound++;
            total_io_bound_cpu_bursts += static_cast<int>(process.cpu_bursts.size());
            total_io_bound_io_bursts += static_cast<int>(process.io_bursts.size());
            for (const int cpu_burst : process.cpu_bursts) {
                total_cpu_burst_time += cpu_burst;
                total_io_bound_cpu_burst_time += cpu_burst;
                total_cpu_bursts++;
            }
            for (const int io_burst : process.io_bursts) {
                total_io_burst_time += io_burst;
                total_io_bound_io_burst_time += io_burst;
                total_io_bursts++;
            }
        }
    }

    const double avg_cpu_bound_cpu_burst = (total_cpu_bound_cpu_bursts > 0) ? static_cast<double>(total_cpu_bound_cpu_burst_time) / total_cpu_bound_cpu_bursts : 0;
    const double avg_io_bound_cpu_burst = (total_io_bound_cpu_bursts > 0) ? static_cast<double>(total_io_bound_cpu_burst_time) / total_io_bound_cpu_bursts : 0;
    const double overall_avg_cpu_burst = (total_cpu_bursts > 0) ? static_cast<double>(total_cpu_burst_time) / total_cpu_bursts : 0;
    const double avg_cpu_bound_io_burst = (total_cpu_bound_io_bursts > 0) ? static_cast<double>(total_cpu_bound_io_burst_time) / total_cpu_bound_io_bursts : 0;
    const double avg_io_bound_io_burst = (total_io_bound_io_bursts > 0) ? static_cast<double>(total_io_bound_io_burst_time) / total_io_bound_io_bursts : 0;
    const double overall_avg_io_burst = (total_io_bursts > 0) ? static_cast<double>(total_io_burst_time) / total_io_bursts : 0;

    outfile << fixed << setprecision(3);
    outfile << "-- number of processes: " << processes.size() << endl;
    outfile << "-- number of CPU-bound processes: " << num_cpu_bound << endl;
    outfile << "-- number of I/O-bound processes: " << num_io_bound << endl;
    outfile << "-- CPU-bound average CPU burst time: " << ceil(avg_cpu_bound_cpu_burst * 1000.0) / 1000.0 << " ms" << endl;
    outfile << "-- I/O-bound average CPU burst time: " << ceil(avg_io_bound_cpu_burst * 1000.0) / 1000.0 << " ms" << endl;
    outfile << "-- overall average CPU burst time: " << ceil(overall_avg_cpu_burst * 1000.0) / 1000.0 << " ms" << endl;
    outfile << "-- CPU-bound average I/O burst time: " << ceil(avg_cpu_bound_io_burst * 1000.0) / 1000.0 << " ms" << endl;
    outfile << "-- I/O-bound average I/O burst time: " << ceil(avg_io_bound_io_burst * 1000.0) / 1000.0 << " ms" << endl;
    outfile << "-- overall average I/O burst time: " << ceil(overall_avg_io_burst * 1000.0) / 1000.0 << " ms" << endl;

}

void print_processes(const vector<Process>& processes, const int num_processes, const int num_cpu_bound, const int seed, const double lambda) {
    cout << "<<< PROJECT PART I" << endl;
    cout << "<<< -- process set (n=" << num_processes << ") with " << num_cpu_bound << " CPU-bound process" << (num_cpu_bound > 1 ? "es" : "") << endl;
    cout << "<<< -- seed=" << seed << "; lambda=" << fixed << setprecision(6) << lambda << "; bound=" << bound << endl;

    for (const Process& process : processes) {
        cout << (process.is_cpu_bound() ? "CPU-bound" : "I/O-bound") << " process " << process.id << ": arrival time " << process.arrival_time << "ms; "
             << process.cpu_bursts.size() << " CPU burst" << (process.cpu_bursts.size() == 1 ? "" : "s") << endl;
    }
}

void print_event(const int time, const string& event, const queue<Process*>& ready_queue) {
    cout << "time " << time << "ms: " << event << " [Q";
    if (ready_queue.empty()) {
        cout << " empty";
    } else {
        queue<Process*> temp_queue = ready_queue;
        while (!temp_queue.empty()) {
            cout << " " << temp_queue.front()->id;
            temp_queue.pop();
        }
    }
    cout << "]" << endl;
}

void sort_ready_queue(queue<Process*>& rq) {
    vector<Process*> temp;
    while (!rq.empty()) {
        temp.push_back(rq.front());
        rq.pop();
    }
    sort(temp.begin(), temp.end(), [](const Process* a, const Process* b) {
        if (a->tau == b->tau) {
            return a->id < b->id; // Lexicographical order by process ID
        }
        return a->tau < b->tau;
    });
    for (Process* p : temp) {
        rq.push(p);
    }
}

// Function to run the First-Come, First-Served (FCFS) simulation
// Parameters:
// - processes: vector of Process objects representing all processes
// - ready_queue: queue of Process pointers representing the ready queue
// - tcs: context switch time in milliseconds
void run_fcfs_simulation(vector<Process>& processes, queue<Process*>& ready_queue, const int tcs) {
    int current_time = 0; // Current simulation time in milliseconds
    bool cpu_idle = true; // Flag to indicate if the CPU is idle
    Process* current_process = nullptr; // Pointer to the current process using the CPU
    int cpu_burst_remaining = 0; // Remaining time for the current CPU burst
    queue<Process*> io_queue; // Queue of processes waiting for I/O
    unordered_set<string> completed_processes; // Set of completed process IDs
    int context_switch_time = 0; // Time remaining for the current context switch

    // Print the start of the simulation
    print_event(current_time, "Simulator started for FCFS", ready_queue);

    // Main simulation loop
    while (completed_processes.size() < processes.size() || !ready_queue.empty() || !io_queue.empty() || !cpu_idle) {
        // Check for process arrivals
        for (auto & processe : processes) {
            if (processe.arrival_time == current_time && completed_processes.find(processe.id) == completed_processes.end()) {
                ready_queue.push(&processe);
                print_event(current_time, "Process " + processe.id + " arrived; added to ready queue", ready_queue);
            }
        }

        // Check for I/O completions
        vector<Process*> temp_io_queue;
        while (!io_queue.empty()) {
            Process* io_process = io_queue.front();
            io_queue.pop();
            if (io_process->io_bursts.front() == current_time) {
                io_process->io_bursts.erase(io_process->io_bursts.begin());
                ready_queue.push(io_process);
                print_event(current_time, "Process " + io_process->id + " completed I/O; added to ready queue", ready_queue);
            } else {
                temp_io_queue.push_back(io_process);
            }
        }
        for (Process* io_process : temp_io_queue) {
            io_queue.push(io_process);
        }

        // If CPU is idle and ready queue is not empty, start the next process
        if (cpu_idle && !ready_queue.empty()) {
            current_process = ready_queue.front();
            ready_queue.pop();
            cpu_burst_remaining = current_process->cpu_bursts.front();
            current_process->cpu_bursts.erase(current_process->cpu_bursts.begin());
            cpu_idle = false;
            context_switch_time = tcs / 2;
            current_process->context_switches++;
            print_event(current_time + context_switch_time, "Process " + current_process->id + " started using the CPU for " + to_string(cpu_burst_remaining) + "ms burst", ready_queue);
        }

        // If a process is running, decrement its burst time
        if (!cpu_idle && current_process != nullptr) {
            if (context_switch_time > 0) {
                context_switch_time--;
                current_time++;
            } else {
                if (cpu_burst_remaining == 0) {
                    if(!current_process->cpu_bursts.empty()){
                        print_event(current_time, "Process " + current_process->id + " completed a CPU burst; " + to_string(current_process->cpu_bursts.size())
                           + (current_process->cpu_bursts.size() == 1 ? " burst" : " bursts") + " to go", ready_queue);
                    }
                    if (!current_process->cpu_bursts.empty()) {
                        current_time += tcs / 2;
                        current_process->io_bursts.front() += current_time;
                        current_process->total_cpu_time += cpu_burst_remaining;
                        current_process->total_time = current_time - current_process->arrival_time;
                        current_process->turnaround_time = current_process->total_time;
                        io_queue.push(current_process);
                        print_event(current_time - tcs/2, "Process " + current_process->id + " switching out of CPU; blocking on I/O until time " + to_string(current_process->io_bursts.front()) + "ms", ready_queue);
                    } else {
                        print_event(current_time, "Process " + current_process->id + " terminated", ready_queue);
                        current_time += tcs / 2;
                        completed_processes.insert(current_process->id);
                        current_process->total_cpu_time += cpu_burst_remaining;
                        current_process->total_time = current_time - current_process->arrival_time;
                        current_process->turnaround_time = current_process->total_time;
                    }
                    cpu_idle = true;
                    current_process = nullptr;
                    context_switch_time = tcs / 2;
                } else {
                    cpu_burst_remaining--;
                    current_time++;
                }
            }
        } else {
            current_time++;
        }
    }

    // Print the end of the simulation
    print_event(current_time, "Simulator ended for FCFS", ready_queue);
    cout << endl;
}

void run_sjf_simulation(vector<Process>& processes, queue<Process*>& ready_queue, const int tcs, const double alpha) {
    int current_time = 0; // Current simulation time in milliseconds
    bool cpu_idle = true; // Flag to indicate if the CPU is idle
    Process* current_process = nullptr; // Pointer to the current process using the CPU
    int cpu_burst_remaining = 0; // Remaining time for the current CPU burst
    queue<Process*> io_queue; // Queue of processes waiting for I/O
    unordered_set<string> completed_processes; // Set of completed process IDs
    int context_switch_time = 0; // Time remaining for the current context switch

    // Print the start of the simulation
    print_event(current_time, "Simulator started for SJF", ready_queue);

    int tau = 0; // Variable to store the recalculated tau value
    while (completed_processes.size() < processes.size() || !ready_queue.empty() || !io_queue.empty() || !cpu_idle) {
        // Check for process arrivals
        for (auto & process : processes) {
            if (process.arrival_time == current_time && completed_processes.find(process.id) == completed_processes.end()) {
                ready_queue.push(&process); // Add process to the ready queue
                sort_ready_queue(ready_queue); // Sort the ready queue based on tau values
                print_event(current_time, "Process " + process.id + " (tau " + to_string(static_cast<int>(process.tau)) + "ms) arrived; added to ready queue", ready_queue);
            }
        }

        // Check for I/O completions
        vector<Process*> temp_io_queue;
        while (!io_queue.empty()) {
            Process* io_process = io_queue.front();
            io_queue.pop();
            if (io_process->io_bursts.front() == current_time) {
                io_process->io_bursts.erase(io_process->io_bursts.begin()); // Remove the completed I/O burst
                ready_queue.push(io_process); // Add process back to the ready queue
                sort_ready_queue(ready_queue); // Sort the ready queue based on tau values
                print_event(current_time, "Process " + io_process->id + " (tau " + to_string(static_cast<int>(io_process->tau)) + "ms) completed I/O; added to ready queue", ready_queue);
            } else {
                temp_io_queue.push_back(io_process);
            }
        }
        for (Process* io_process : temp_io_queue) {
            io_queue.push(io_process);
        }

        // If CPU is idle and ready queue is not empty, start the next process
        if (cpu_idle && !ready_queue.empty()) {
            current_process = ready_queue.front();
            ready_queue.pop();
            cpu_burst_remaining = current_process->cpu_bursts.front();
            current_process->cpu_bursts.erase(current_process->cpu_bursts.begin());
            cpu_idle = false;
            context_switch_time = tcs / 2;
            print_event(current_time + context_switch_time, "Process " + current_process->id + " (tau " + to_string(static_cast<int>(current_process->tau)) + "ms) started using the CPU for " + to_string(cpu_burst_remaining) + "ms burst", ready_queue);
            tau = ceil(alpha * cpu_burst_remaining + (1 - alpha) * current_process->tau); // Recalculate tau using exponential averaging
        }

        // If a process is running, decrement its burst time
        if (!cpu_idle && current_process != nullptr) {
            if (context_switch_time > 0) {
                context_switch_time--;
                current_time++;
            } else {
                if (cpu_burst_remaining == 0) {
                    if (!current_process->cpu_bursts.empty()) {
                        print_event(current_time, "Process " + current_process->id + " (tau " + to_string(static_cast<int>(current_process->tau)) + "ms) completed a CPU burst; " + to_string(current_process->cpu_bursts.size())
                           + (current_process->cpu_bursts.size() == 1 ? " burst" : " bursts") + " to go", ready_queue);
                        // Update tau using exponential averaging
                        const double old_tau = current_process->tau;
                        current_process->tau = tau;
                        print_event(current_time, "Recalculated tau for process " + current_process->id + ": old tau " + to_string(static_cast<int>(old_tau)) + "ms ==> new tau " + to_string(static_cast<int>(current_process->tau)) + "ms", ready_queue);
                    }
                    if (!current_process->cpu_bursts.empty()) {
                        // Process has more CPU bursts, move it to I/O queue
                        current_time += tcs / 2;
                        current_process->io_bursts.front() += current_time;
                        io_queue.push(current_process);
                        print_event(current_time - tcs / 2, "Process " + current_process->id + " switching out of CPU; blocking on I/O until time " + to_string(current_process->io_bursts.front()) + "ms", ready_queue);
                    } else {
                        // Process has no more CPU bursts, it is terminated
                        print_event(current_time, "Process " + current_process->id + " terminated", ready_queue);
                        current_time += tcs / 2;
                        completed_processes.insert(current_process->id);
                    }

                    cpu_idle = true;
                    current_process = nullptr;
                    context_switch_time = tcs / 2;
                } else {
                    cpu_burst_remaining--;
                    current_time++;
                }
            }
        } else {
            current_time++;
        }
    }

    // Print the end of the simulation
    print_event(current_time, "Simulator ended for SJF", ready_queue);
    cout << endl;
}

// void run_srt_simulation(vector<Process>& processes, queue<Process*>& ready_queue, const int tcs, const double alpha) {
//     int current_time = 0;
//     bool cpu_idle = true;
//     Process* current_process = nullptr;
//     int cpu_burst_remaining = 0;
//     queue<Process*> io_queue;
//
//     print_event(current_time, "Simulator started for SRT", ready_queue);
//
//     // SRT simulation logic here
//
//     print_event(current_time, "Simulator ended for SRT", ready_queue);
//     cout << endl;
//
// }
//
// void run_rr_simulation(vector<Process>& processes, queue<Process*>& ready_queue, const int tcs, const double alpha, const int tslice) {
//     int current_time = 0;
//     bool cpu_idle = true;
//     Process* current_process = nullptr;
//     int cpu_burst_remaining = 0;
//     queue<Process*> io_queue;
//
//     print_event(current_time, "Simulator started for RR", ready_queue);
//
//     // RR simulation logic here
//
//     print_event(current_time, "Simulator ended for RR", ready_queue);
// }

void reset_simulation(vector<Process>& processes, const vector<Process>& original_processes) {
    processes = original_processes;
    ready_queue = queue<Process*>();
}

int main(const int argc, char* argv[]) {

    //disable bufferd output
    setbuf(stdout, nullptr);


    if (argc != 9) {
        cerr << "Usage: " << argv[0] << " <num_processes> <num_cpu_bound> <seed> <lambda> <bound> <tcs> <alpha> <tslice>" << endl;
        return 1;
    }

    const int num_processes = static_cast<int>(strtol(argv[1], nullptr, 10));
    //Invalid number of processes
    if((num_processes<=0) || num_processes>260){
      fprintf(stderr,"ERROR: Number of processes must be greater than 0 and less than 261.\n");
      return EXIT_FAILURE;
    }

    char * p;
    int cpu_bound_processes = static_cast<int>(strtol(argv[2], &p,10));
    //input wasnt an int
    if(*p){
      fprintf(stderr,"ERROR: cpu_bound_processes must be an int.\n");
      return EXIT_FAILURE;
    }

    //Invalid number of cpu bound processes
    if(cpu_bound_processes<0){
      fprintf(stderr,"ERROR: Number of CPU-bound processes must be positive.\n");
      return EXIT_FAILURE;
    }

    const long int seed = strtol(argv[3], &p,10);
    if(*p){
      fprintf(stderr,"ERROR: Seed must be an int.\n");
      return EXIT_FAILURE;
    }

    const double lambda = strtod(argv[4], &p);
    if(*p){
      fprintf(stderr,"ERROR: Lambda must be a number.\n");
      return EXIT_FAILURE;
    }

    if(lambda <= 0.0){
      fprintf(stderr,"ERROR: Lambda must be greater than 0.\n");
      return EXIT_FAILURE;
    }

    bound = static_cast<int>(strtol(argv[5],&p,10));
    if(*p){
      fprintf(stderr,"ERROR: Upper bound must be a number.\n");
      return EXIT_FAILURE;
    }

    if(bound <=0 ){
      fprintf(stderr,"ERROR: Upper bound must be greater than 0.\n");
      return EXIT_FAILURE;
    }

    tcs = static_cast<int>(strtol(argv[6],&p,10));
    if(*p || tcs <= 0 || tcs % 2 != 0){
        fprintf(stderr,"ERROR: tcs must be a positive even integer.\n");
        return EXIT_FAILURE;
    }

    alpha = strtod(argv[7],&p);
    if(*p || alpha < 0.0 || alpha > 1.0){
        fprintf(stderr,"ERROR: alpha must be a floating-point value in the range [0, 1].\n");
        return EXIT_FAILURE;
    }

    tslice = static_cast<int>(strtol(argv[8],&p,10));
    if(*p || tslice <= 0){
        fprintf(stderr,"ERROR: tslice must be a positive integer.\n");
        return EXIT_FAILURE;
    }

    srand48(seed);

    vector<Process> processes;
    initialize_processes(processes, num_processes, cpu_bound_processes, lambda);
    print_processes(processes, num_processes,  cpu_bound_processes, static_cast<int>(seed), lambda);


    cout << endl;

    cout << "<<< PROJECT PART II" << endl;
    cout << "<<< -- t_cs=" << tcs << "ms; alpha=" << fixed << setprecision(2) << alpha << "; t_slice=" << tslice << "ms" << endl;



    const vector<Process> original_processes = processes;
    ofstream outfile("simout.txt");
    calculate_and_write_statistics(processes, outfile);

    run_fcfs_simulation(processes, ready_queue, tcs);
    write_algorithm_to_file("FCFS", outfile, processes);
    reset_simulation(processes, original_processes);

    run_sjf_simulation(processes, ready_queue, tcs, alpha);
    reset_simulation(processes, original_processes);

    // run_srt_simulation(processes, ready_queue, tcs, alpha);
    // reset_simulation(processes, original_processes);
    //
    // run_rr_simulation(processes, ready_queue, tcs, alpha, tslice);



    outfile.close();
    return 0;
}