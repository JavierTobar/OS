//#include "interpreter.h"
#include "shellmemory.h"
#include "shell.h"
#include "pcb.h"
#include "ram.h"
#include "cpu.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct ReadyQueue
{
	struct PCB *head;
	struct PCB *tail;
} readyQueue;

void roundRobin();
void terminate();
void addToReady(struct PCB *pcb);

int main(int argc, const char *argv[])
{
	shellUI();
}

void myinit(char *filename)
{
	int start = 0, end = 0; // dummy values, they'll get overwritten in addToRAM
	char buffer[100];				//assumes max of 100 char per line
	FILE *file = fopen(filename, "r");
	if (file == NULL) // this SHOULD never fail since it's been guaranteed to exist in exec()
	{
		printf("Error opening the file from kernel.c \n");
		return;
	}
	else
	{
		addToRAM(file, &start, &end); // we're passing pointers so they'll be properly updated in addToRAM
		fclose(file);									// no longer needed
		struct PCB *pcb = makePCB(start, end);
		addToReady(pcb);
	}
}

void addToReady(struct PCB *pcb)
{
	if (readyQueue.head == NULL)
	{
		readyQueue.head = pcb;
		return; // we don't assign value to tail
	}
	else
	{
		struct PCB **traverser = &(readyQueue.head)->next;
		while (*traverser)
		{
			traverser = &(*traverser)->next;
		}
		*traverser = pcb;
		readyQueue.tail = *traverser;
	}
}

void scheduler()
{
	CPU.IP = (readyQueue.head)->PC;
	if (CPU.IP == 0) // if statement for the very first program
	{
		if ((readyQueue.head->end - readyQueue.head->PC) >= 2) // i.e. we pass 2 full quantas
		{
			if (run(2) == 1)
			{
				terminate();
				return;
			}
			(readyQueue.head)->PC += 2;
			roundRobin();
			return;
		}
		else if ((readyQueue.head->end - readyQueue.head->PC) == 1)
		{
			if (run(1) == 1)
				terminate();
			(readyQueue.head)->PC++;
			terminate();
			return;
		}
		else if ((readyQueue.head->end - readyQueue.head->PC) == 0)
		{
			if (run(1) == 1)
				terminate();
			return; // 0 means 1 instruction left
		}
	}
	if (CPU.IP > 0) // to check if CPU is available : quanta finished or nothing assigned to it
	{
		// checks if you pass 2 quanta or less, depending on how close the program is to ending
		if ((readyQueue.head->end - readyQueue.head->PC) >= 2) // i.e. we pass 2 full quantas
		{
			if (run(2) == 1)
				terminate();
			(readyQueue.head)->PC += 2;
			roundRobin();
			return;
		}
		else if ((readyQueue.head->end - readyQueue.head->PC) == 1)
		{
			if (run(2) == 1)
				terminate(); // 1 means 2 instructions left
			return;
		}
		else if ((readyQueue.head->end - readyQueue.head->PC) == 0)
		{
			if (run(1) == 1)
				terminate();
			return;
		}
	}
}

// this will always do the same, i.e. place head in the back of queue and move other programs up
void roundRobin()
{
	struct PCB *tmp = readyQueue.head;
	if (tmp->next == NULL)
	{
		scheduler(); // i.e. only one program in ready queue
	}
	else if (tmp->next == readyQueue.tail) // i.e. only two programs in ready queue
	{
		readyQueue.head = readyQueue.tail;
		readyQueue.tail = tmp;
		scheduler();
	}
	else
	{

		// this will only be reached if all 3 programs are in ready queue
		readyQueue.head = tmp->next;
		readyQueue.tail = tmp;
		tmp->next = NULL;
		scheduler();
	}
}

// this will terminate the program, this will always be called when the head of readyQueue has finished
void terminate()
{
	if ((readyQueue.head)->next == NULL) // i.e. only one program left
	{
		free(readyQueue.head);
		readyQueue.head = NULL;
		for (int i = 0; i < 1000; i++)
			ram[i] = NULL;
		return; // doesnt call scheduler, returns back to interpreter();
	}
	struct PCB *tmp = (readyQueue.head)->next;
	free(readyQueue.head);
	readyQueue.head = tmp;
	scheduler();
}