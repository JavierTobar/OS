#ifndef PCB_H
#define PCB_H

extern int pidCounter;

struct PCB
{
    int PC;
    int start; // DEPRECATED SINCE WE'VE IMPLEMENTED VIRTUAL MEMORY
    int end; // DEPRECATED SINCE WE'VE IMPLEMENTED VIRTUAL MEMORY
    int PID;
    int pageTable[10];
    int PC_page;
    int PC_offset;
    int pages_max;
    struct PCB *next; // one of many possible solutions to traverse the ready list
};

/**
 * Mallocs a pcb and initializes it accordingly
 *
 * @param start where the pcb starts in ram, this is 0 now since we've implemented virtual memory
 * @param end where the pcb ends in ram, this is 0 now since we've implemented virtual memory
 * @return pcb
 */
struct PCB* makePCB(int start, int end);

#endif