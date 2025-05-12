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
int set_io_request_random(Processor* processor){
    if(is_idle(processor)) return 0;
    int temp1=rand()%100+1;
    int temp2=rand()%MAX_IO_REQUEST_TIME+1;
    if(temp1<IO_REQUEST_PROBABILITY) {
        processor->executing_process->IO_request=true;
        processor->executing_process->remaining_IO_burst_time=temp2;
    }
    else{
        processor->executing_process->IO_request=false;
        processor->executing_process->remaining_IO_burst_time=0;
    }
    return processor->executing_process->pid;
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
