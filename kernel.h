#ifndef KERNEL_H
#define KERNEL_H

/*
	Creates a PCB from the fileName
	adds it to the ReadyQueue
*/
void myinit(char *filename);

/*
	Scheduler assigns quanta to each process
	Note : we only assign a maximum of 2 quanta
	If process is done executing then terminate
	else call roundRobin scheduling on the readyQueue
*/
void scheduler();

/*
	Adds PCB to the back of the PCB linked-list (readyQueue)
*/
void addToReady(struct PCB *pcb);

/*
	This will terminate (free) the current head of the readyQueue 
	It will also take care of pointers that were pointing to this allocated memory
*/
void terminate();

/*
	our ReadyQueue is a linked-list with two pointers :
	One to the head of the linked-list and one to the tail of the linked-list
	These pointers allow us to do roundRobin scheduling in O(n) rather than O(2n)
	Note : these complexities are both linear and this was simply a design choice
*/
struct ReadyQueue
{
	struct PCB *head;
	struct PCB *tail;
} readyQueue;




#endif
