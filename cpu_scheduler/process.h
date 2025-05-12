#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifndef _PROCESS_HEATHER
#define _PROCESS_HEATHER
#define MAX_BURST_TIME 20
#define MAX_ARRIVAL_TIME 50
#define MAX_PRIORITY 7
#define MAX_PROCESS_NUMBER 100
typedef struct {
    unsigned int pid;
    unsigned int cpu_burst_time;
    unsigned int arrival_time;
    unsigned int remaining_CPU_burst_time;
    unsigned int remaining_IO_burst_time;
    unsigned int waiting_time;
    int priority; //smaller the number, higher the priority
    bool IO_request;
}PCB;
bool is_file_exist(const char* filename);

//check if all process of process_list finished
bool is_all_process_finished(PCB process_list[],int number_of_process);

//set remaining cpu time, reset waiting time
int reset_all_process(PCB process_list[],int number_of_process);
//make csv file 
int make_process_list(int number_of_process, const char* filename);
//file must follow this csv format: (burst time),(arrival time),(priority)\n
int load_process_list(PCB process_list[],const char* filename);
void print_pcb(PCB pcb);
#endif