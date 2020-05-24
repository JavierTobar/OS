#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

struct PCB
{
    int PC;
    int start; // no longer used since using virtual memory
    int end; // no longer used since using virtual memory
    int PID;
    int pageTable[10];
    int PC_page;
    int PC_offset;
    int pages_max;
    struct PCB *next; // one of many possible solutions to traverse the ready list
};

int pidCounter;

struct PCB *makePCB(int start, int end)
{
    struct PCB *pcb = malloc(sizeof(struct PCB));
    if (pcb == NULL)
    {
        printf("Memory allocation error for PCB\n");
        return NULL;
    }

    // design decision, when pageTable has -1 it simply means that it's empty
    for (int i = 0; i < 10; i++)
    {
        pcb->pageTable[i] = -1;
    }

    pcb->PID = ++pidCounter; // we want to start our pid at 1, not 0
    pcb->PC = start;
    pcb->start = start;
    pcb->end = end;
    pcb->next = NULL;
    pcb->PC_page = 0;
    pcb->PC_offset = 0;
    pcb->pages_max = 0; // will be overwritten in memorymanager
    pcb->next = NULL;

    return pcb;
}