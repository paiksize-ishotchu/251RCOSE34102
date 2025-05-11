#include "queue.h"
Queue* create_queue(void){
    Queue* queue=(Queue*)malloc(sizeof(Queue));
    if (queue==NULL) return NULL;
    queue->in=0;
    queue->out=0;
    queue->number_of_element=0;
    queue->head=(PCB**)malloc(sizeof(PCB*)*QUEUE_SIZE);
    if (queue->head==NULL){
        free(queue);
        return NULL;
    }
    return queue;
}
int destruct_queue(Queue** queue){
    if(*queue==NULL) return -1;
    if((*queue)->head!=NULL) free((*queue)->head);
    (*queue)->head=NULL;
    free(*queue);
    *queue=NULL;
    return 0;
}
int enqueue_queue(Queue* queue, PCB* pcb){
    if(queue->number_of_element==QUEUE_SIZE) return -1;
    else{
        queue->head[queue->in]=pcb;
        queue->in=(queue->in+1)%QUEUE_SIZE;
        queue->number_of_element++;
    }
    return 0;
}
PCB* dequeue_queue(Queue* queue){
    if(is_empty_queue(queue)) return NULL;
    else{
        PCB* pcb_ptr=NULL;
        pcb_ptr=queue->head[queue->out];
        queue->out=(queue->out+1)%QUEUE_SIZE;
        queue->number_of_element--;
        return pcb_ptr;
    }
}
bool is_empty_queue(Queue* queue){
    if(queue->number_of_element==0) return true;
    else return false;
}
void travel_queue(Queue* queue){
    if(is_empty_queue(queue)) printf("this queue is empty\n");
    for(int i=0;i<queue->number_of_element;++i){
        print_pcb(*queue->head[queue->out+i]);
    }
}