#include <stdio.h>
#include <string.h>
#include "process.h"
#include "processor.h"
#include "queue.h"
#include "simulator.h"
#define FILENAME_DEFAULT "process_list.csv"
bool silent=false;
int load(PCB process_array[]);
void show(PCB process_array[],int number_of_process);
int make(int number_of_process);
void help();
void test(PCB process_array[],int number_of_process, int* log[]);
void chart(int* log[]);
void toggle_silent();
int main(int argc, char* argv[]){
    srand(time(NULL));
    printf("\n\t\t\tWelcome to my CPU scheduling simulator!\n");help();
    PCB process_array[MAX_PROCESS_NUMBER];
    int number_of_process=0;
    int cpu_log[2000]={-1,};
    int IO_log[2000]={-1,};
    int* log[2]={cpu_log,IO_log};
    while(true){
        char command[10];
        printf(">>");
        scanf("%s",command);
        if(strcmp(command,"exit")==0) break;
        else if(strcmp(command,"help")==0) help();
        else if(strcmp(command,"make")==0) make(number_of_process);
        else if(strcmp(command,"load")==0) number_of_process=load(process_array);
        else if(strcmp(command,"show")==0) show(process_array,number_of_process);
        else if(strcmp(command,"test")==0) test(process_array,number_of_process,log);
        else if(strcmp(command,"chart")==0) chart(log);
        else if(strcmp(command,"silent")==0) toggle_silent();
        else printf("wrong command...(type 'help' to see command list!)\n");
    }
    return 0;
}
void help(){
    printf("\n%14s: show this\n%14s: generate process data file\n%14s: load process data file\n%14s: terminate program\n","help","make","load","exit");
    printf("%14s: show current processes\n%14s: simulate current process\n%14s: show gannt chart of last test\n","show","test","chart");
    printf("%14s: toggle verbose (default=on)\n","silent");
}
void toggle_silent(){
    if(silent){
        printf("loud from now on\n");
        silent=!silent;
    }
    else{
        printf("silent from now on\n");
        silent=!silent;
    }
    
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
void test(PCB process_array[],int number_of_process, int* log[]){
    char buf[20];
    printf("choose scheduling algorithm : ");
    scanf("%s",buf);
    if(strcmp(buf,"FCFS")==0||strcmp(buf,"fcfs")==0) simulate(process_array,number_of_process,log,FCFS);
    else if(strcmp(buf,"SJF")==0||strcmp(buf,"sjf")==0) simulate(process_array,number_of_process,log,SJF);
    else if(strcmp(buf,"RR")==0||strcmp(buf,"rr")==0) simulate(process_array,number_of_process,log,RR);
    else printf("I don't know that algorithm...\n");
}
void chart(int* log[]){
    int i=0;
    printf("%8s%8s%8s\n","time","CPU","IO");
    while(log[0][i]!=-1){
        printf("[%04d] :",i);
        if(log[0][i]==0) printf("%8s","IDLE");
        else printf("%8d",log[0][i]);
        if(log[1][i]==0) printf("%8s\n","IDLE");
        else printf("%8d\n",log[1][i]);
        i++;
    }
    printf("[%04d] :%10s\n",i,"END");
}