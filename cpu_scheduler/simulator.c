#include "simulator.h"
Simulation* initialize_simulation(PCB process_list[],int number_of_process,int* log[],algorithm policy){
    Simulation* simul=(Simulation*)malloc(sizeof(Simulation));
    if(simul==NULL) return NULL;
    simul->current_time=0;
    simul->process_list=process_list;
    simul->number_of_process=number_of_process;
    simul->cpu_log=log[0];
    simul->IO_log=log[1];
    reset_all_process(simul->process_list,number_of_process);
    simul->CPU=initialize_processor(CPU);
    simul->IO_device=initialize_processor(IO);
    if(simul->CPU==NULL&&simul->IO_device==NULL){
        //initializing CPU and IO fails
        return NULL;
    }
    else if(simul->CPU==NULL&&simul->IO_device!=NULL){
        //initializing CPU fails but IO success
        destruct_processor(&simul->IO_device);
        return NULL;
    }
    else if(simul->CPU!=NULL&&simul->IO_device==NULL){
        //initializing CPU success but IO fails
        destruct_processor(&simul->CPU);
        return NULL;    
    }
    Queue* ready_queue=create_queue();
    Queue* waiting_queue=create_queue();
    if(ready_queue==NULL&&waiting_queue==NULL){
        //creating ready queue and waiting queue fails
        return NULL;
    }
    else if(ready_queue!=NULL&&waiting_queue==NULL){
        //creating ready queue success but waiting queue fails
        destruct_queue(&ready_queue);
        return NULL;
    }
    else if(ready_queue==NULL&&waiting_queue!=NULL){
        //creating ready queue fails but waiting queue success
        destruct_queue(&waiting_queue);
        return NULL;
    }
    set_scheduling_algorithm(simul->CPU,policy);
    simul->CPU->ready_queue=ready_queue;
    simul->CPU->waiting_queue=waiting_queue;
    simul->IO_device->ready_queue=simul->CPU->waiting_queue;
    simul->IO_device->waiting_queue=simul->CPU->ready_queue;
    return simul;
}
void simulate(PCB process_array[], int number_of_process, int* log[], algorithm policy,int time_quantum){
    if(number_of_process==0) return;
    Simulation* simul=initialize_simulation(process_array,number_of_process,log,policy);
    if(simul->CPU->scheduling_policy==RR) simul->CPU->time_quantum=time_quantum;
    while(!is_all_process_finished(process_array,number_of_process))
        update_simulation(simul);
    simul->cpu_log[simul->current_time]=-1;
    simul->IO_log[simul->current_time]=-1;
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
    check_IO_request(simul);
    update_process(simul);
    write_log(simul);
    if(!silent) print_simulation_state(simul);
    execute_process(simul);
    update_waiting_time(simul);
    random_IO_request(simul);
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
        while((simul->cpu_log)[j]!=-1){
            if((simul->cpu_log)[j]==i+1) finish_time=j;
            j++;
        }
        val+=finish_time-((simul->process_list)[i].arrival_time)+1;
    }
    printf("Average Turnaround Time : %lf\n",val/simul->number_of_process);
}
bool check_IO_request(Simulation* simul){
    if(is_idle(simul->CPU)) return false;
    else if(simul->CPU->executing_process->IO_request){
        stop_process(simul->CPU);
        simul->CPU->executing_process=NULL;
        return true;
    }
    return false;
}
void random_IO_request(Simulation* simul){
    set_io_request_random(simul->CPU);
}
void print_simulation_state(Simulation* simul){
    printf("\n[%04d]: CPU - \n",simul->current_time);
    if(is_idle(simul->CPU)) printf("%14s%s\n","","IDLE");
    else print_pcb(*simul->CPU->executing_process);
    printf("\tIO  - \n");
    if(is_idle(simul->IO_device)) printf("%14s%s\n","","IDLE");
    else print_pcb(*simul->IO_device->executing_process);
    printf("\n%14s","");
    printf("----------READY QUEUE-----------\n");
    travel_queue(simul->CPU->ready_queue);
    printf("\n%14s","");
    printf("----------WAIT QUEUE------------\n");
    travel_queue(simul->CPU->waiting_queue);
    printf("\n\n");
}
void write_log(Simulation* simul){
    if(is_idle(simul->CPU)) simul->cpu_log[simul->current_time]=0;
    else simul->cpu_log[simul->current_time]=simul->CPU->executing_process->pid;
    if(is_idle(simul->IO_device)) simul->IO_log[simul->current_time]=0;
    else simul->IO_log[simul->current_time]=simul->IO_device->executing_process->pid;
}
void admit_process(Simulation* simul){
    for(int i=0;i<simul->number_of_process;++i){
        if((simul->process_list[i]).arrival_time==simul->current_time) enqueue_queue(simul->CPU->ready_queue,&(simul->process_list)[i]);
    }
}
void execute_process(Simulation* simul){
    if(!is_idle(simul->CPU)) {
        simul->CPU->executing_process->remaining_CPU_burst_time--;
        simul->CPU->timer++;
    }
    if(!is_idle(simul->IO_device)) simul->IO_device->executing_process->remaining_IO_burst_time--;
}
void update_waiting_time(Simulation* simul){
    Queue* ready_queue=simul->CPU->ready_queue;
    for(int i=0;i<ready_queue->number_of_element;++i){
        ready_queue->head[ready_queue->out+i]->waiting_time++;
    }
}
void update_process(Simulation* simul){
    dispatch_process(simul->CPU);
    dispatch_process(simul->IO_device);
    //for case: process terminates->CPU sits idle->CPU dispatch fails since ready queue is empty->
    //->IO stop->now ready queue is not empty 
    //consequently CPU and IO both IDLE while there is a process in the ready queue
    //thus dispatch for CPU one more time. 
    dispatch_process(simul->CPU);
}