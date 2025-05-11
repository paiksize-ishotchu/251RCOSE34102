#include <stdio.h>
#include <string.h>
#include "process.h"
#include "processor.h"
#include "queue.h"
#include "simulator.h"
#define FILENAME_DEFAULT "process_list.csv"
int load(PCB process_array[]);
void show(PCB process_array[],int number_of_process);
int make(int number_of_process);
void help();
void test(PCB process_array[],int number_of_process, int cpu_log[]);
void chart(int cpu_log[]);
int main(int argc, char* argv[]){
    printf("\n\t\t\tWelcome to my CPU scheduling simulator!\n");help();
    PCB process_array[MAX_PROCESS_NUMBER];
    int number_of_process=0;
    int cpu_log[2000]={-1,};
    while(true){
        char command[10];
        printf(">>");
        scanf("%s",command);
        if(strcmp(command,"exit")==0) break;
        else if(strcmp(command,"help")==0) help();
        else if(strcmp(command,"make")==0) make(number_of_process);
        else if(strcmp(command,"load")==0) number_of_process=load(process_array);
        else if(strcmp(command,"show")==0) show(process_array,number_of_process);
        else if(strcmp(command,"test")==0) test(process_array,number_of_process,cpu_log);
        else if(strcmp(command,"chart")==0) chart(cpu_log);
        else printf("wrong command...(type 'help' to see command list!)\n");
    }
    return 0;
}
void help(){
    printf("\n\thelp: show this\n\tmake: generate process data file\n\tload: load process data file\n\texit: terminate program\n");
    printf("\tshow: list process initial state\n\ttest: simulate current process\n\tchart: show gannt chart of last test\n");
}
int make(int number_of_process){
    char filename[100];
    printf("filename ('d' for default):");
    scanf("%s",filename);
    if(strcmp(filename,"d")==0) strcpy(filename,FILENAME_DEFAULT);
    else{
        printf("how many processes? : ");
        scanf("%d",&number_of_process);
    }
    return make_process_list(number_of_process,filename);
}
int load(PCB process_array[]){
    char filename[100];
    printf("filename ('d' for default):");
    scanf("%s",filename);
    if(strcmp(filename,"d")==0) strcpy(filename,FILENAME_DEFAULT);
    return load_process_list(process_array,filename);
}
void show(PCB process_array[],int number_of_process){
    if(number_of_process==0) printf("please load process file first\n");
    for(int i=0;i<number_of_process;++i) print_pcb(process_array[i]);
}
void test(PCB process_array[],int number_of_process, int cpu_log[]){
    char buf[20];
    printf("choose scheduling algorithm : ");
    scanf("%s",buf);
    if(strcmp(buf,"FCFS")==0||strcmp(buf,"fcfs")==0) simulate(process_array,number_of_process,cpu_log,FCFS);
    else printf("I don't know that algorithm...\n");
}
void chart(int cpu_log[]){
    int i=0;
    while(cpu_log[i]!=-1){
        if(cpu_log[i]==0) printf("[%04d] : IDLE\n",i);
        else printf("[%04d] : %03d\n",i,cpu_log[i]);
        i++;
    }
    printf("[%04d] : END\n",i);
}