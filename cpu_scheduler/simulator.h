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
    int* log;
    PCB* process_list;
    Processor* CPU;
    Processor* IO_device;
}Simulation;
void simulate(PCB process_array[], int number_of_process,int log[], algorithm policy);
Simulation* initialize_simulation(PCB process_list[],int number_of_process,int log[],algorithm policy);
int destruct_simulation(Simulation** simul);
void update_simulation(Simulation* simul);
void write_log(Simulation* simul);
void check_IO_request(Simulation* simul);
void print_average_waitingtime(Simulation* simul);
void print_average_turnaround(Simulation* simul);
void update_waiting_time(Simulation* simul);
void admit_process(Simulation* simul);
void update_running_process(Simulation* simul);
void update_running_process_FCFS(Simulation* simul);
#endif