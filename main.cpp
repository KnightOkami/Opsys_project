#include <iostream>
#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double next_exp(double lambda, long int bound);
void nextName(char &name, int &num);
void printProcess(std::string p, bool CPU_or_IO, int arrival, int burst, long int bound, double lambda);

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
    std::cout<<processes<<" "<< cpu_bound_processes<< " "<< seed << " "<< lambda<< " "<< bound<< "\n";

    // cpu bound processes first
    //cpu proccess are named A0-A9, B0...Z9
    char cpu_name = 'A';
    int cpu_num = 0;
    for(int i = 0; i < cpu_bound_processes; i++){
      double arrival = floor(next_exp(lambda,bound));
      double burst = ceil(drand48()*32);
      std::string s = std::string(1,cpu_name) + std::to_string(cpu_num);
      nextName(cpu_name,cpu_num);
      
      printProcess(s, 1, arrival, burst, bound, lambda);
    }
    for(int j = 0; j < processes - cpu_bound_processes; j++){
      double arrival = floor(next_exp(lambda,bound));
      double burst = ceil(drand48()*32);
      std::string s = std::string(1,cpu_name) + std::to_string(cpu_num);
      nextName(cpu_name,cpu_num);
      
      printProcess(s, 0, arrival, burst, bound, lambda);
    }
    
    //This will be fun
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

  void printProcess(std::string p, bool CPU_or_IO, int arrival, int burst, long int bound, double lambda){
    //1 for CPU, 0 for IO
    if(CPU_or_IO){
      std::cout<<"CPU-bound process "<< p << ": arrival time " <<arrival<<"ms; "<<burst<<" CPU bursts:\n";
      for(int i = 0; i < burst - 1; i++){
        int burst_time = ceil(next_exp(lambda, bound)) * 4;
        int io_time =  ceil(next_exp(lambda, bound));
        std::cout<<"==> CPU burst "<<burst_time <<"ms ==> I/O burst "<<io_time<<"ms"<<std::endl;
      }
      int burst_time = ceil(next_exp(lambda, bound)) * 4;
      std::cout<<"==> CPU burst "<<burst_time<<"ms"<<std::endl;
    }
    else{
      std::cout<<"I/O-bound process "<< p << ": arrival time " <<arrival<<"ms; "<<burst<<" CPU bursts:\n";
      for(int i = 0; i < burst - 1; i++){
        int burst_time = ceil(next_exp(lambda, bound));
        int io_time = ceil(next_exp(lambda,bound)) * 8;
        std::cout<<"==> CPU burst "<<burst_time<<"ms ==> I/O burst "<<io_time<<"ms"<<std::endl;
      }
      int burst_time = ceil(next_exp(lambda, bound));
      std::cout<<"==> CPU burst "<<burst_time<<"ms"<<std::endl;

    }
  }


