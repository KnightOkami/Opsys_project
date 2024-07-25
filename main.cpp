#include <iostream>
#include <queue>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <fstream>
#include "Process.h"

using namespace std;

queue<Process*> ready_queue;
int bound = 0;

double next_exp(double lambda) {
    double r = drand48();
    return -log(r) / lambda;
}

void generate_bursts(Process& process, double lambda) {
    int num_cpu_bursts = ceil(drand48() * 32);
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

void initialize_processes(vector<Process>& processes, int num_processes, int num_cpu_bound, double lambda) {
    char id_letter = 'A';
    int id_number = 0;

    for (int i = 0; i < num_processes; ++i) {
        string id = string(1, id_letter) + to_string(id_number);

        int arrival_time = 0;
        double r;
        do {
            r = next_exp(lambda);
        } while (r > bound);
        arrival_time = floor(r);

        ProcessType type = (i < num_cpu_bound) ? CPU_BOUND : IO_BOUND;
        Process process(id, arrival_time, type);

        generate_bursts(process, lambda);

        processes.push_back(process);
        ready_queue.push(&processes.back());

        id_number++;
        if (id_number > 9) {
            id_number = 0;
            id_letter++;
        }
    }
}

void calculate_statistics(const vector<Process>& processes, int& num_cpu_bound, int& num_io_bound, double& avg_cpu_bound_cpu_burst, double& avg_io_bound_cpu_burst, double& overall_avg_cpu_burst, double& avg_cpu_bound_io_burst, double& avg_io_bound_io_burst, double& overall_avg_io_burst) {
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

    for(const auto& process : processes) {
        if (process.is_cpu_bound()) {
            num_cpu_bound++;
            total_cpu_bound_cpu_bursts += process.cpu_bursts.size();
            total_cpu_bound_io_bursts += process.io_bursts.size();
            for (int cpu_burst : process.cpu_bursts) {
                total_cpu_burst_time += cpu_burst;
                total_cpu_bound_cpu_burst_time += cpu_burst;
                total_cpu_bursts++;
            }
            for (int io_burst : process.io_bursts) {
                total_io_burst_time += io_burst;
                total_cpu_bound_io_burst_time += io_burst;
                total_io_bursts++;
            }
        } else {
            num_io_bound++;
            total_io_bound_cpu_bursts += process.cpu_bursts.size();
            total_io_bound_io_bursts += process.io_bursts.size();
            for (int cpu_burst : process.cpu_bursts) {
                total_cpu_burst_time += cpu_burst;
                total_io_bound_cpu_burst_time += cpu_burst;
                total_cpu_bursts++;
            }
            for (int io_burst : process.io_bursts) {
                total_io_burst_time += io_burst;
                total_io_bound_io_burst_time += io_burst;
                total_io_bursts++;
            }
        }
    }

    avg_cpu_bound_cpu_burst = (total_cpu_bound_cpu_bursts > 0) ? (double)total_cpu_bound_cpu_burst_time / total_cpu_bound_cpu_bursts : 0;
    avg_io_bound_cpu_burst = (total_io_bound_cpu_bursts > 0) ? (double)total_io_bound_cpu_burst_time / total_io_bound_cpu_bursts : 0;
    overall_avg_cpu_burst = (total_cpu_bursts > 0) ? (double)total_cpu_burst_time / total_cpu_bursts : 0;
    avg_cpu_bound_io_burst = (total_cpu_bound_io_bursts > 0) ? (double)total_cpu_bound_io_burst_time / total_cpu_bound_io_bursts : 0;
    avg_io_bound_io_burst = (total_io_bound_io_bursts > 0) ? (double)total_io_bound_io_burst_time / total_io_bound_io_bursts : 0;
    overall_avg_io_burst = (total_io_bursts > 0) ? (double)total_io_burst_time / total_io_bursts : 0;
}

void write_statistics_to_file(int num_processes, int num_cpu_bound, int num_io_bound, double avg_cpu_bound_cpu_burst, double avg_io_bound_cpu_burst, double overall_avg_cpu_burst, double avg_cpu_bound_io_burst, double avg_io_bound_io_burst, double overall_avg_io_burst) {
    ofstream outfile("simout.txt");
    outfile << fixed << setprecision(3);
    outfile << "-- number of processes: " << num_processes << endl;
    outfile << "-- number of CPU-bound processes: " << num_cpu_bound << endl;
    outfile << "-- number of I/O-bound processes: " << num_io_bound << endl;
    outfile << "-- CPU-bound average CPU burst time: " << ceil(avg_cpu_bound_cpu_burst * 1000.0) / 1000.0 << " ms" << endl;
    outfile << "-- I/O-bound average CPU burst time: " << ceil(avg_io_bound_cpu_burst * 1000.0) / 1000.0 << " ms" << endl;
    outfile << "-- overall average CPU burst time: " << ceil(overall_avg_cpu_burst * 1000.0) / 1000.0 << " ms" << endl;
    outfile << "-- CPU-bound average I/O burst time: " << ceil(avg_cpu_bound_io_burst * 1000.0) / 1000.0 << " ms" << endl;
    outfile << "-- I/O-bound average I/O burst time: " << ceil(avg_io_bound_io_burst * 1000.0) / 1000.0 << " ms" << endl;
    outfile << "-- overall average I/O burst time: " << ceil(overall_avg_io_burst * 1000.0) / 1000.0 << " ms" << endl;
    outfile.close();
}

void print_processes(const vector<Process>& processes, int num_processes, int num_cpu_bound, int seed, double lambda) {
    cout << "<<< PROJECT PART I" << endl;
    cout << "<<< -- process set (n=" << num_processes << ") with " << num_cpu_bound << " CPU-bound process" << (num_cpu_bound > 1 ? "es" : "") << endl;
    cout << "<<< -- seed=" << seed << "; lambda=" << fixed << setprecision(6) << lambda << "; bound=" << bound << endl;

    for (const auto& process : processes) {
        cout << (process.is_cpu_bound() ? "CPU-bound" : "I/O-bound") << " process " << process.id << ": arrival time " << process.arrival_time << "ms; "
             << process.cpu_bursts.size() << " CPU burst" << (process.cpu_bursts.size() == 1 ? ":" : "s:") << endl;
        for (size_t j = 0; j < process.cpu_bursts.size(); ++j) {
            cout << "==> CPU burst " << process.cpu_bursts[j] << "ms";
            if (j < process.io_bursts.size()) {
                cout << " ==> I/O burst " << process.io_bursts[j] << "ms";
            }
            cout << endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        cerr << "Usage: " << argv[0] << " <num_processes> <num_cpu_bound> <seed> <lambda> <bound>" << endl;
        return 1;
    }

     //Incorrect amount of arguments
    if(argc!=6){
      fprintf(stderr,"ERROR: Incorrect number of arguments.\n");
      return EXIT_FAILURE;
    }

    int processes = atoi(argv[1]);
    //Invalid number of processes
    if((processes<=0) || processes>260){
      fprintf(stderr,"ERROR: Number of processes must be greater than 0 and less than 261.\n");
      return EXIT_FAILURE;
    }
    
    char * p;
    int cpu_bound_processes = strtol(argv[2], &p,10);
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
     
    long int seed = strtol(argv[3], &p,10);
    if(*p){
      fprintf(stderr,"ERROR: Seed must be an int.\n");
      return EXIT_FAILURE;
    }

    double lambda = strtod(argv[4], &p);
    if(*p){
      fprintf(stderr,"ERROR: Lambda must be a number.\n");
      return EXIT_FAILURE;
    }

    if(lambda <= 0.0){
      fprintf(stderr,"ERROR: Lambda must be greater than 0.\n");
      return EXIT_FAILURE;
    }

    long int bound = strtol(argv[5],&p,10);
    if(*p){
      fprintf(stderr,"ERROR: Upper bound must be a number.\n");
      return EXIT_FAILURE;
    }

    if(bound <=0 ){
      fprintf(stderr,"ERROR: Upper bound must be greater than 0.\n");
      return EXIT_FAILURE;
    }

    srand48(seed);

    vector<Process> processes;
    initialize_processes(processes, num_processes, num_cpu_bound, lambda);
    print_processes(processes, num_processes, num_cpu_bound, seed, lambda);

    num_cpu_bound = 0;
    int num_io_bound = 0;
    double avg_cpu_bound_cpu_burst = 0;
    double avg_io_bound_cpu_burst = 0;
    double overall_avg_cpu_burst = 0;
    double avg_cpu_bound_io_burst = 0;
    double avg_io_bound_io_burst = 0;
    double overall_avg_io_burst = 0;

    calculate_statistics(processes, num_cpu_bound, num_io_bound, avg_cpu_bound_cpu_burst, avg_io_bound_cpu_burst, overall_avg_cpu_burst, avg_cpu_bound_io_burst, avg_io_bound_io_burst, overall_avg_io_burst);
    write_statistics_to_file(num_processes, num_cpu_bound, num_io_bound, avg_cpu_bound_cpu_burst, avg_io_bound_cpu_burst, overall_avg_cpu_burst, avg_cpu_bound_io_burst, avg_io_bound_io_burst, overall_avg_io_burst);

    return 0;
}