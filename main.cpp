#include <iostream>
#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double next_exp(double lambda, long int bound);
void nextName(char &name, int &num);
void printProcess(std::string p, bool CPU_or_IO, int arrival, int burst, long int bound, double lambda,int & cpu_total_cpu_burst,int & cpu_total_io_burst,int & io_total_cpu_burst,
    int & io_total_io_burst);
void printFile(int io_total_cpu_burst,int io_total_io_burst,int cpu_total_cpu_burst,int cpu_total_io_burst,int io_io_burst_count, int io_cpu_burst_count,int cpu_cpu_burst_count,int cpu_io_burst_count,int processes,int cpu_bound_processes);

int main(int argc, char * argv[]) {

   

    ///////Input Error Checking///////

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


    //start of program
    srand48(seed);
    std::cout<<"<<< PROJECT PART I\n";
    if(cpu_bound_processes==1){
       std::cout<<"<<< -- process set (n="<<processes<<") with "<<cpu_bound_processes<<" CPU-bound process\n";
    }
    else{
       std::cout<<"<<< -- process set (n="<<processes<<") with "<<cpu_bound_processes<<" CPU-bound processes\n";
    }
   

    std::cout<<"<<< -- seed="<<seed<<"; ";
    fprintf(stdout,"lambda=%.6f; bound=%ld\n",lambda,bound);


    // cpu bound processes first
    //cpu proccess are named A0-A9, B0...Z9

    int cpu_total_cpu_burst =0;
    int cpu_total_io_burst=0;
    int io_total_cpu_burst =0; 
    int io_total_io_burst =0;
    int io_io_burst_count=0;
    int io_cpu_burst_count=0;
    int cpu_cpu_burst_count=0;
    int cpu_io_burst_count=0;



    char cpu_name = 'A';
    int cpu_num = 0;
    for(int i = 0; i < cpu_bound_processes; i++){
      double arrival = floor(next_exp(lambda,bound));
      double burst = ceil(drand48()*32);
      std::string s = std::string(1,cpu_name) + std::to_string(cpu_num);
      nextName(cpu_name,cpu_num);
      cpu_cpu_burst_count+=burst;
      cpu_io_burst_count+=burst-1;
      
      printProcess(s, 1, arrival, burst, bound, lambda,cpu_total_cpu_burst,cpu_total_io_burst,io_total_io_burst,io_total_cpu_burst);
    }
    for(int j = 0; j < processes - cpu_bound_processes; j++){
      double arrival = floor(next_exp(lambda,bound));
      double burst = ceil(drand48()*32);
      std::string s = std::string(1,cpu_name) + std::to_string(cpu_num);
      nextName(cpu_name,cpu_num);
      io_cpu_burst_count+=burst;
      io_io_burst_count+=burst-1;
      
      printProcess(s, 0, arrival, burst, bound, lambda,cpu_total_cpu_burst,cpu_total_io_burst,io_total_cpu_burst,io_total_io_burst);
    }
    
    //This will be fun
    
    
    printFile( io_total_cpu_burst, io_total_io_burst, cpu_total_cpu_burst, cpu_total_io_burst, io_io_burst_count,  io_cpu_burst_count, cpu_cpu_burst_count, cpu_io_burst_count, processes, cpu_bound_processes);
    
    return 0;
}
  double next_exp(double lambda, long int bound){
    double r = drand48();
    double x = -log(r)/lambda;    
    while(x > bound){
      r = drand48();
      x = -log(r)/lambda;
    }
    return x;
  }
  
  void nextName(char &name, int &num){
    if(num == 9){
      name++;
      num = 0;
    }
    else{
      num++;
    }
  }
  void printProcess(std::string p, bool CPU_or_IO, int arrival, int burst, long int bound, double lambda, int &cpu_total_cpu_burst,int &cpu_total_io_burst, int & io_total_cpu_burst, int & io_total_io_burst){
    //1 for CPU, 0 for IO
    if(CPU_or_IO){
      if(burst==1){
        std::cout<<"CPU-bound process "<< p << ": arrival time " <<arrival<<"ms; "<<burst<<" CPU burst:\n";
      }else{
      std::cout<<"CPU-bound process "<< p << ": arrival time " <<arrival<<"ms; "<<burst<<" CPU bursts:\n";
      }
      for(int i = 0; i < burst - 1; i++){
        int burst_time = ceil(next_exp(lambda, bound)) * 4;
        int io_time =  ceil(next_exp(lambda, bound));
        cpu_total_cpu_burst+=burst_time;
        cpu_total_io_burst+=io_time;
        std::cout<<"==> CPU burst "<<burst_time <<"ms ==> I/O burst "<<io_time<<"ms"<<std::endl;
      }
      int burst_time = ceil(next_exp(lambda, bound)) * 4;
      cpu_total_cpu_burst+=burst_time;
      std::cout<<"==> CPU burst "<<burst_time<<"ms"<<std::endl;
    }
    else{
      if(burst==1){
        std::cout<<"I/O-bound process "<< p << ": arrival time " <<arrival<<"ms; "<<burst<<" CPU burst:\n";
      }
      else{
        std::cout<<"I/O-bound process "<< p << ": arrival time " <<arrival<<"ms; "<<burst<<" CPU bursts:\n";
      }
      
      for(int i = 0; i < burst - 1; i++){
        int burst_time = ceil(next_exp(lambda, bound));
        int io_time = ceil(next_exp(lambda,bound)) * 8;
        std::cout<<"==> CPU burst "<<burst_time<<"ms ==> I/O burst "<<io_time<<"ms"<<std::endl;
        io_total_io_burst+=io_time;
        io_total_cpu_burst+=burst_time;
      }
      int burst_time = ceil(next_exp(lambda, bound));
      io_total_cpu_burst+=burst_time;
      std::cout<<"==> CPU burst "<<burst_time<<"ms"<<std::endl;

    }
  }


void printFile(int io_total_cpu_burst,int io_total_io_burst,int cpu_total_cpu_burst,int cpu_total_io_burst,int io_io_burst_count, int io_cpu_burst_count,int cpu_cpu_burst_count,int cpu_io_burst_count,int processes,int cpu_bound_processes){
  int io_processes = processes - cpu_bound_processes;
  FILE* file = fopen("simout.txt", "w");
   if (file == nullptr) {
        fprintf(stderr,"ERROR: opening file\n");
    }

  fprintf(file,"-- number of processes: %d\n-- number of CPU-bound processes: %d\n-- number of I/O-bound processes: %d\n",processes,cpu_bound_processes,io_processes);
 
  float avg_cpu_cpu =cpu_cpu_burst_count?(ceil((cpu_total_cpu_burst * 1000.0) / cpu_cpu_burst_count) / 1000.0):0.f;
  float avg_io_cpu =io_cpu_burst_count?(ceil((io_total_cpu_burst * 1000.0) / io_cpu_burst_count) / 1000.0):0.f;
  float avg_cpu=(io_cpu_burst_count+cpu_cpu_burst_count)?(ceil(((io_total_cpu_burst+cpu_total_cpu_burst) * 1000.0) / (io_cpu_burst_count+cpu_cpu_burst_count)) / 1000.0):0.f;
  float avg_cpu_io=cpu_io_burst_count?(ceil((cpu_total_io_burst * 1000.0) / cpu_io_burst_count) / 1000.0):0.f;
  float avg_io_io=io_io_burst_count?(ceil((io_total_io_burst * 1000.0) / io_io_burst_count) / 1000.0):0.f;
  float avg_io=(cpu_io_burst_count+io_io_burst_count)?(ceil(((io_total_io_burst+cpu_total_io_burst) * 1000.0) / (cpu_io_burst_count+io_io_burst_count)) / 1000.0):0.f;

  fprintf(file,"-- CPU-bound average CPU burst time: %.3f ms\n-- I/O-bound average CPU burst time: %.3f ms\n-- overall average CPU burst time: %.3f ms\n-- CPU-bound average I/O burst time: %.3f ms\n-- I/O-bound average I/O burst time: %.3f ms\n-- overall average I/O burst time: %.3f ms\n",avg_cpu_cpu,avg_io_cpu,avg_cpu,avg_cpu_io,avg_io_io,avg_io);


  fclose(file);
}
