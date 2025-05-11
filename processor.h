#include "queue.h"
#include "process.h"
#ifndef _PROCESSOR_HEATHER
#define _PROCESSOR_HEATHER
typedef enum{
    FCFS,
    SJF
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
}Processor;
Processor* initialize_processor(processor_type type);
int destruct_processor(Processor** processor);
int set_scheduling_algorithm(Processor* processor, algorithm policy);
int stop_process(Processor* processor);
bool is_idle(Processor* processor);
PCB* dispatch_process(Processor* processor);
PCB* dispatch_FCFS(Queue* ready_queue);
#endif