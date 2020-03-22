#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

struct PCB 
{
	int PC;
	int start;
	int end;
	struct PCB *next; // one of many possible solutions to traverse the ready list
};

struct PCB* makePCB(int start, int end)
{
	struct PCB *pcb = malloc(sizeof(struct PCB));
	if (pcb == NULL)
	{
		printf("Memory allocation error for PCB\n");
		return NULL;
	}

	pcb->PC = start;
	pcb->start = start;
	pcb->end = end;
	pcb->next = NULL;

	return pcb;
}
