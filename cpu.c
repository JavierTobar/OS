#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "ram.h"
#include "pcb.h"
#include "interpreter.h"
#include "kernel.h"
#include "memorymanager.h"

struct CPU
{
    int IP;
    char IR[1000];
    int quanta;
    int offset;
} CPU;

int run(int quanta)
{
    if (CPU.offset == 4) // then we have a pageFault and we need to fix it
    {
        // segmentation fault here somewhere
        CPU.offset = 0; // we also need to put pcb's offset to 0
        readyQueue.head->PC_offset = 0;
        readyQueue.head->PC_page++;

        if (readyQueue.head->PC_page == readyQueue.head->pages_max) // we don't have any more pages in our program
        {
            return 1; // terminate
        }
        if (readyQueue.head->pageTable[readyQueue.head->PC_page] != -1) // i.e frame exists
        {
            readyQueue.head->PC = readyQueue.head->pageTable[readyQueue.head->PC_page];
            // we don't bother giving CPU the IP since this pcb is going to the back of list following a pseudo-interrupt
            return 2;
        } else // frame doesn't exist, we need to load and take care of potential victim
        {
            struct PCB *pcb = readyQueue.head;
            int frame = findFrame();


            if (frame != -1) // i.e. we found a frame
            {
                int pid = pcb->PID;
                char str[40];
                sprintf(str, "./BackingStore/%d.txt", pid);
                FILE *fileInBackingStore = fopen(str, "w");
                loadPage(pcb->PC_page, fileInBackingStore, frame); // we load the page
                pcb->pageTable[pcb->PC_page] = frame;
                updatePageTable(pcb, pcb->PC_page, frame, -1);
                pcb->PC_page++;
                readyQueue.head->PC = readyQueue.head->pageTable[readyQueue.head->PC_page];
                return 2;
            } else // we have a victimFrame
            {
                int pid = pcb->PID;
                char str[40];
                sprintf(str, "./BackingStore/%d.txt", pid);
                FILE *fileInBackingStore = fopen(str, "w");
                if (fileInBackingStore == NULL)
                {
                    return 1; // something went wrong, we just terminate the process
                }
                int victimFrame = findVictim(pcb);
                loadPage(pcb->PC_page, fileInBackingStore, victimFrame);
                pcb->pageTable[pcb->PC_page] = victimFrame;
                updatePageTable(pcb, pcb->PC_page, victimFrame, victimFrame);
                pcb->PC_page++;
                readyQueue.head->PC = frame;
                return 2;
            }
        }
    }

    while (quanta > 0)
    {
//	    printf("CPU.IP : %d and CPU.Offset : %d \n",CPU.IP, CPU.offset);
//        printf("%s \n", ram[CPU.IP*4 + CPU.offset]);


        // unexpected possible behaviour
        if (ram[CPU.IP * 4 + CPU.offset] == NULL)
        {
            printf("Something went wrong executing PCB with PID : %d \n", readyQueue.head->PID);
            printf("Terminating the process... \n");
            return 1;
        }

        strcpy(CPU.IR, ram[CPU.IP * 4 + CPU.offset]);


        if (strncmp(CPU.IR, "quit", 4) == 0) // if quit, script terminates, but not program
        {
            return 1; // this will let scheduler know to call terminate on this script
        } else
        {
            interpret(CPU.IR);
            quanta--;
            CPU.offset++;

        }
    }
    CPU.quanta = 2; // reset for next call
    return 0;
}