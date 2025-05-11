#include "simulator.h"
Simulation* initialize_simulation(PCB process_list[],int number_of_process,int log[],algorithm policy){
    Simulation* simul=(Simulation*)malloc(sizeof(Simulation));
    if(simul==NULL) return NULL;
    simul->current_time=0;
    simul->number_of_process=number_of_process;
    simul->CPU=initialize_processor(CPU);
    simul->IO_device=initialize_processor(IO);
    simul->process_list=process_list;
    if(simul->CPU==NULL||simul->IO_device==NULL) return NULL;
    Queue* ready_queue=create_queue();
    Queue* waiting_queue=create_queue();
    if(ready_queue==NULL||waiting_queue==NULL) return NULL;
    simul->CPU->ready_queue=ready_queue;
    simul->CPU->waiting_queue=waiting_queue;
    simul->IO_device->ready_queue=simul->CPU->waiting_queue;
    simul->IO_device->waiting_queue=simul->CPU->ready_queue;
    simul->log=log;
    set_scheduling_algorithm(simul->CPU,policy);
    reset_all_process(simul->process_list,number_of_process);
    return simul;
}
void simulate(PCB process_array[], int number_of_process, int log[], algorithm policy){
    if(number_of_process==0) return;
    Simulation* simul=initialize_simulation(process_array,number_of_process,log,policy);
    while(!is_all_process_finished(process_array,number_of_process)){
        update_simulation(simul);
    }
    simul->log[simul->current_time-1]=-1;
    print_average_waitingtime(simul);
    print_average_turnaround(simul);
    destruct_simulation(&simul);
}
int destruct_simulation(Simulation** simul){
    if(*simul==NULL) return -1;
    destruct_processor(&(*simul)->CPU);
    (*simul)->IO_device->ready_queue=NULL;
    (*simul)->IO_device->waiting_queue=NULL;
    destruct_processor(&(*simul)->IO_device);
    free(*simul);
    *simul=NULL;
    return 0;
}
void update_simulation(Simulation* simul){
    admit_process(simul);
    update_running_process(simul);
    update_waiting_time(simul);
    write_log(simul);
    simul->current_time++;
}
void print_average_waitingtime(Simulation* simul){
    double val=0;
    for(int i=0;i<simul->number_of_process;++i){
        val+=(double)(simul->process_list)[i].waiting_time;
    }
    printf("Average Waiting Time: %lf\n",val/simul->number_of_process);
}
void print_average_turnaround(Simulation* simul){
    double val=0;
    for(int i=0;i<simul->number_of_process;++i){
        int finish_time=0;
        int j=0;
        while((simul->log)[j]!=-1){
            if((simul->log)[j]==i+1) finish_time=j;
            j++;
        }
        val+=finish_time-((simul->process_list)[i].arrival_time)+1;
    }
    printf("Average Turnaround Time : %lf\n",val/simul->number_of_process);
}
void write_log(Simulation* simul){
    if(is_idle(simul->CPU)) simul->log[simul->current_time]=0;
    else simul->log[simul->current_time]=simul->CPU->executing_process->pid;
}
void admit_process(Simulation* simul){
    for(int i=0;i<simul->number_of_process;++i){
        if((simul->process_list[i]).arrival_time==simul->current_time) enqueue_queue(simul->CPU->ready_queue,&(simul->process_list)[i]);
    }
}
void update_waiting_time(Simulation* simul){
    Queue* ready_queue=simul->CPU->ready_queue;
    for(int i=0;i<ready_queue->number_of_element;++i){
        ready_queue->head[ready_queue->out+i]->waiting_time++;
    }
}
void update_running_process(Simulation* simul){
    if(!is_idle(simul->CPU)) simul->CPU->executing_process->remaining_CPU_burst_time--;
    switch(simul->CPU->scheduling_policy){
        case FCFS:
            update_running_process_FCFS(simul);
            break;
        default:
            break;

    }
}
void update_running_process_FCFS(Simulation* simul){
    PCB* next_process=NULL;
    if(is_idle(simul->CPU))
        next_process=dispatch_FCFS(simul->CPU->ready_queue);
    else if(is_process_finished(*simul->CPU->executing_process)){
        stop_process(simul->CPU);
        next_process=dispatch_FCFS(simul->CPU->ready_queue);
    }
    else next_process=simul->CPU->executing_process;
    simul->CPU->executing_process=next_process;
}
