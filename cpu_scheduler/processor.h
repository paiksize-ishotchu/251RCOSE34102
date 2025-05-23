#include "queue.h"
#include "process.h"
#ifndef _PROCESSOR_HEATHER
#define _PROCESSOR_HEATHER
#define MAX_IO_REQUEST_TIME 10
typedef enum{
    //negative number for preemptive algorithm
    SJF_PREEMPTIVE=-2,
    PRIORITY_PREEMPTIVE,

    FCFS,
    SJF,
    PRIORITY,
    //10 or higher for timer related algorithm
    RR=10,
    LOTTERY
}algorithm;
typedef enum{
    CPU,
    IO
}processor_type;
typedef struct{
    PCB* executing_process;
    Queue* ready_queue;
    Queue* waiting_queue;
    algorithm scheduling_policy;
    processor_type type;
    unsigned int time_quantum;
    unsigned int timer;
}Processor;
Processor* initialize_processor(processor_type type);
int destruct_processor(Processor** processor);
int set_scheduling_algorithm(Processor* processor, algorithm policy);
int stop_process(Processor* processor);
int set_io_request_random(Processor* processor);
bool is_idle(Processor* processor);
bool is_process_finished(Processor* processor);
bool is_preemptive(Processor* processor);
bool is_time_expired(Processor* Processor);
void reset_timer(Processor* Processor);
//dispatch a process from ready queue according to processor's scheduling policy
void dispatch_process(Processor* processor);
PCB* dispatch_FCFS(Processor* processor);
PCB* dispatch_SJF(Processor* processor);
PCB* dispatch_PRIORITY(Processor* processor);
PCB* dispatch_RR(Processor* processor);
PCB* dispatch_LOTTERY(Processor* processor);
#endif