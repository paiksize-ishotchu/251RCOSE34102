#include "processor.h"
Processor* initialize_processor(processor_type type){
    Processor* processor=(Processor*)malloc(sizeof(Processor));
    if(processor==NULL) return NULL;
    processor->executing_process=NULL;
    processor->ready_queue=NULL;
    processor->waiting_queue=NULL;
    processor->scheduling_policy=FCFS;
    processor->type=type;
    return processor;
}
int destruct_processor(Processor** processor){
    if(*processor==NULL) return -1;
    if((*processor)->ready_queue!=NULL) destruct_queue(&(*processor)->ready_queue);
    (*processor)->ready_queue=NULL;
    if((*processor)->waiting_queue!=NULL) destruct_queue(&(*processor)->waiting_queue);
    (*processor)->waiting_queue=NULL;
    free(*processor);
    *processor=NULL;
    return 0;
}
int set_scheduling_algorithm(Processor* processor, algorithm policy){
    processor->scheduling_policy=policy;
    return 0;
}
bool is_idle(Processor* processor){
    return processor->executing_process==NULL;
}
int stop_process(Processor* processor){
    if(processor->executing_process==NULL) return -1;
    if(processor->type==CPU){
        if(processor->executing_process->remaining_CPU_burst_time==0) return 1;
        else if(processor->executing_process->IO_request){
            enqueue_queue(processor->waiting_queue,processor->executing_process);
        }
        else enqueue_queue(processor->ready_queue,processor->executing_process);
    }
    else if(processor->type==IO){
        if(processor->executing_process->remaining_IO_burst_time==0)
            enqueue_queue(processor->waiting_queue,processor->executing_process);
        else return -1;
    }
    return 0;
}
PCB* dispatch_process(Processor* processor){
    switch (processor->scheduling_policy)
    {
    case FCFS:
        return dispatch_FCFS(processor->ready_queue);
        break;
    
    default:
        break;
    }
    return NULL;
}
PCB* dispatch_FCFS(Queue* ready_queue){
    return dequeue_queue(ready_queue);
}
