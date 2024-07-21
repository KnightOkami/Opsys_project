#include <iostream>
#include <cstdlib>


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



    std::cout<<processes<<" "<< cpu_bound_processes<< " "<< seed << " "<< lambda<< " "<< bound<< "\n";


    
    
    std::cout << argc;
    //This will be fun
    return 0;
}
