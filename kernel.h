#ifndef KERNEL_H
#define KERNEL_H

void myinit(char *filename);
void scheduler();
void addToReady(struct PCB *pcb);
void terminate();
struct ReadyQueue
{
	struct PCB *head;
	struct PCB *tail;
} readyQueue;




#endif
