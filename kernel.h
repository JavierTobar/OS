#ifndef KERNEL_H
#define KERNEL_H

/*
	Creates a PCB from the fileName
	adds it to the ReadyQueue
*/
/**
 * Creates a PCB from the fileName
 *
 * @param filename  The name of the file
 * @return pcb created from filename or NULL if file not found
 */
struct PCB *myinit(char *filename);

/**
 * Executes the pcbs in readyQueue using round robin scheduling
 */
void scheduler();

/**
 * Adds a pcb to the linked-list readyQueue
 *
 * @param pcb   The pcb to add
 */
void addToReady(struct PCB *pcb);

/**
 * Terminates the head of the readyQueue
 */
void terminate();

struct ReadyQueue
{
	struct PCB *head;
	struct PCB *tail;
} readyQueue;

#endif