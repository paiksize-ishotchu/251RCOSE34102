#include "process.h"
bool is_file_exist(const char* filename){
    FILE *fp=fopen(filename,"r");
    if(fp==NULL) return false;
    else {
        fclose(fp);
        return true;
    }
}
int make_process_list(int number_of_process, const char* filename){
    if(is_file_exist(filename)){
        printf("%s already exists\n",filename);
        return -1;
    }
    else{
        FILE *fp=fopen(filename,"w");
        for(int i=0;i<number_of_process&&i<MAX_PROCESS_NUMBER;++i){
            //csv file: busrt_time, arrival_time, priority
            fprintf(fp,"%d,%d,%d\n",rand()%MAX_BURST_TIME+1,rand()%MAX_ARRIVAL_TIME+1,rand()%MAX_PRIORITY);
        }
        fclose(fp);
        return 0;
    }
}
int load_process_list(PCB process_list[],const char* filename){
    if(is_file_exist(filename)==false){
        printf("%s does not exist\n",filename);
        return -1;
    }
    else{
        FILE *fp=fopen(filename,"r");
        int count=0;
        while(count<MAX_PROCESS_NUMBER&&feof(fp)==0){
            int temp[3]={0};
            fscanf(fp,"%d,%d,%d\n",&temp[0],&temp[1],&temp[2]);
            PCB temp_pcb={
                .pid=count+1,
                .cpu_burst_time=temp[0],
                .arrival_time=temp[1],
                .priority=temp[2],
                .IO_request=false,
                .remaining_CPU_burst_time=temp[0],
                .remaining_IO_burst_time=0,
                .waiting_time=0
            };
            process_list[count]=temp_pcb;
            count++;
        }
        fclose(fp);
        return count;
    }
}
void print_pcb(PCB pcb){
    printf("pid: %03d, burst: %03d, arrival: %03d, priority: %d, remain_cpu: %d, remain_io%d\n",pcb.pid,
        pcb.cpu_burst_time,pcb.arrival_time,pcb.priority,pcb.remaining_CPU_burst_time,pcb.remaining_IO_burst_time);    
}
bool is_process_finished(PCB pcb){
    return pcb.remaining_CPU_burst_time==0;
}
bool is_all_process_finished(PCB process_list[],int number_of_process){
    for(int i=0;i<number_of_process;++i) if(!is_process_finished(process_list[i])) return false;
    return true;
}
int reset_all_process(PCB process_list[],int number_of_process){
    for(int i=0;i<number_of_process;++i) process_list[i].remaining_CPU_burst_time=process_list[i].cpu_burst_time;
    return 0;
}
