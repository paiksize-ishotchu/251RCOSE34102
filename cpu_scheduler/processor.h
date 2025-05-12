#include "queue.h"
#include "process.h"
#ifndef _PROCESSOR_HEATHER
#define _PROCESSOR_HEATHER
#define IO_REQUEST_PROBABILITY 10
#define MAX_IO_REQUEST_TIME 10
typedef enum{
    FCFS,
    SJF,
    RR,
    PRIORITY
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
int set_io_request_random(Processor* processor);
bool is_idle(Processor* processor);
bool is_process_finished(Processor* process);

//dispatch a process from ready queue according to processor's scheduling policy
PCB* dispatch_process(Processor* processor);
PCB* dispatch_FCFS(Queue* ready_queue);
PCB* dispatch_SJF(Queue* ready_queue);
PCB* dispatch_PRIORITY(Queue* ready_queue);

#endif