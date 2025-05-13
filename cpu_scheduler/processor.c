#include "processor.h"
Processor* initialize_processor(processor_type type){
    Processor* processor=(Processor*)malloc(sizeof(Processor));
    if(processor==NULL) return NULL;
    processor->executing_process=NULL;
    processor->ready_queue=NULL;
    processor->waiting_queue=NULL;
    processor->scheduling_policy=FCFS;
    processor->type=type;
    processor->time_quantum=0;
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
    else if(is_process_finished(processor)) return 0;
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
    if(is_idle(processor)) return -1;
    if(processor->type==CPU){
        if(is_process_finished(processor)) 
            //terminate
            return 1;
        else if(processor->executing_process->IO_request){
            //send running process to waiting queue if IO request
            enqueue_queue(processor->waiting_queue,processor->executing_process);
        }
        //send running process to ready queue directly if no IO request(preemption or time exprired)
        else enqueue_queue(processor->ready_queue,processor->executing_process);
    }
    else if(processor->type==IO){
        if(is_process_finished(processor)){
            processor->executing_process->IO_request=false;
            enqueue_queue(processor->waiting_queue,processor->executing_process);
        }
        else return -1;
    }
    processor->executing_process=NULL;
    return 0;
}
bool is_process_finished(Processor* processor){
    if(is_idle(processor)) return false;
    if(processor->type==CPU) return processor->executing_process->remaining_CPU_burst_time==0;
    else if(processor->type==IO) return processor->executing_process->remaining_IO_burst_time==0;
    else return false;
}
bool is_preemptive(Processor* processor){
    return processor->scheduling_policy<0;
}
//dispatch choose next process from ready queue
//if processor is idle, just dispatch
//if task is finished, we can dispatch
//if processor is non-preemptive, keep executing process same
//if processor is preemptive, check if there is better process in ready queue
void dispatch_process(Processor* processor){
   PCB* next_process=NULL;
   PCB* (*dispatch)(Processor*)=NULL;
   switch(processor->scheduling_policy){
        case FCFS:
        dispatch=dispatch_FCFS;
        break;

        case SJF:
        case SJF_PREEMPTIVE:
        dispatch=dispatch_SJF;
        break;

        case PRIORITY:
        case PRIORITY_PREEMPTIVE:
        dispatch=dispatch_PRIORITY;
        break;

        case RR:
        dispatch=dispatch_RR;
        break;

        default:
        break;
    }
    if(is_idle(processor))
        next_process=dispatch(processor);
    else if(is_process_finished(processor)){        
        stop_process(processor);
        next_process=dispatch(processor);
    }
    //skip this if processor is non-preemptive
    else if(is_preemptive(processor)){
        next_process=dispatch(processor);
        //which means 'the ready queue is empty' or 'there is no better process in ready queue' ==> keep going
        if(next_process==NULL) next_process=processor->executing_process;
        else stop_process(processor);
    }
    else next_process=processor->executing_process;
    processor->executing_process=next_process;
}
PCB* dispatch_FCFS(Processor* processor){
    //just use queue FIFO property
    return dequeue_queue(processor->ready_queue,0);
}
PCB* dispatch_SJF(Processor* processor){
    if(is_empty_queue(processor->ready_queue)) return NULL;
    //find min(remain_cpu_burst) process in ready queue
    int best=0;
    int shortest_remain_cpu_burst=__INT_MAX__;
    for(int i=0;i<processor->ready_queue->number_of_element;++i){
        int idx=(processor->ready_queue->out+i)%QUEUE_SIZE;
        int remain_cpu_burst=(processor->ready_queue->head)[idx]->remaining_CPU_burst_time;
        if(remain_cpu_burst<shortest_remain_cpu_burst){
            best=i;
            shortest_remain_cpu_burst=remain_cpu_burst;
        }
    }   
    if(is_idle(processor)) return dequeue_queue(processor->ready_queue,best);
    //if there is no better process
    else if(shortest_remain_cpu_burst>=processor->executing_process->remaining_CPU_burst_time) return NULL;
    //if there is better process
    else return dequeue_queue(processor->ready_queue,best);
}
PCB* dispatch_PRIORITY(Processor* processor){
    if(is_empty_queue(processor->ready_queue)) return NULL;
    int best=0;
    int highest_priority=MAX_PRIORITY;
    for(int i=0;i<processor->ready_queue->number_of_element;++i){
        int idx=(processor->ready_queue->out+i)%QUEUE_SIZE;
        int priority=(processor->ready_queue->head)[idx]->priority;
        if(priority<highest_priority){
            best=i;
            highest_priority=priority;
        }
    }
    if(is_idle(processor)) return dequeue_queue(processor->ready_queue,best);
    else if(PRIORITY>=processor->executing_process->priority) return NULL;
    else return dequeue_queue(processor->ready_queue,best);
}
PCB* dispatch_RR(Processor* processor){

}