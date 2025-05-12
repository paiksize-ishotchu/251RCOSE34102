#include "processor.h"
#include "process.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _SIMULATOR_HEATHER
#define _SIMULATOR_HEATHER
typedef struct{
    unsigned int current_time;
    unsigned int number_of_process;
    int* cpu_log;
    int* IO_log;
    PCB* process_list;
    Processor* CPU;
    Processor* IO_device;
    //CPU and IO_device share the same queues, 
    //CPU's ready queue is used as the IO_device's waiting queue
    //CPU's waiting queue is used as the IO_device's ready queue
}Simulation;
extern bool silent;
void simulate(PCB process_array[], int number_of_process,int* log[], algorithm policy);
Simulation* initialize_simulation(PCB process_list[],int number_of_process,int* log[],algorithm policy);
int destruct_simulation(Simulation** simul);
void update_simulation(Simulation* simul);
void write_log(Simulation* simul);
void check_IO_request(Simulation* simul);
void execute_process(Simulation* simul);
void print_average_waitingtime(Simulation* simul);
void print_average_turnaround(Simulation* simul);
void update_waiting_time(Simulation* simul);
void admit_process(Simulation* simul);
void update_running_process(Simulation* simul);
void update_running_process_FCFS(Processor* processor);
void update_running_process_SJF(Processor* processor);
#endif