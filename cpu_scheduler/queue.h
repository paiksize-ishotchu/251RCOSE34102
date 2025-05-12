//This heather file defines functions that control CIRCULAR QUEUE of PCBS
#include <stdlib.h>
#include <stdbool.h>
#include "process.h"
#ifndef _QUEUE_HEATHER
#define _QUEUE_HEATHER
#define QUEUE_SIZE 100
typedef struct{
    unsigned int number_of_element;
    unsigned int in;
    unsigned int out;
    PCB** head;
}Queue;
Queue* create_queue(void);
int destruct_queue(Queue** queue);
int enqueue_queue(Queue* queue, PCB* pcb);
PCB* dequeue_queue(Queue* queue);
bool is_empty_queue(Queue* queue);
void travel_queue(Queue* queue);
#endif