//#include "interpreter.h"
#include "shellmemory.h"
#include "shell.h"
#include "pcb.h"
#include "ram.h"
#include "cpu.h"
#include "memorymanager.h"
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

void boot();

int kernel();

int main(int argc, const char *argv[])
{
    int error = 0;
    boot();
    error = kernel();
    return error;
}

void boot()
{
    // initialize array to NULL
    for (int i = 0; i < 40; i++)
    {
        ram[i] = NULL;
    }

    // prepare the Backing Store
    system("rm -rf BackingStore"); // you might get error saying that it doesn't exist. this is fine. everything will work.
    system("mkdir BackingStore");
}


int kernel()
{
    return shellUI();
}


struct PCB *myinit(char *filename)
{
    int start = 0, end = 0; // dummy values, they'll get overwritten in addToRAM
    char buffer[100]; //assumes max of 100 char per line
    FILE *file = fopen(filename, "r");
    if (file == NULL) // this SHOULD never fail since it's been guaranteed to exist in exec()
    {
        printf("Error opening the file from kernel.c \n");
        return NULL;
    } else
    {
        //addToRAM(file, &start, &end); // we're passing pointers so they'll be properly updated in addToRAM
        fclose(file); // no longer needed
        struct PCB *pcb = makePCB(start, end);
        addToReady(pcb);
        return pcb;
    }
}


void addToReady(struct PCB *pcb)
{
    if (readyQueue.head == NULL)
    {
        readyQueue.head = pcb;
        return; // we don't assign value to tail
    } else
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

    int pid = readyQueue.head->PID;


    CPU.IP = (readyQueue.head)->pageTable[readyQueue.head->PC_page]; // CPU.IP holds the frame #

    if (CPU.IP ==
        -1) // then it's been victimized and we need to re-load the proper page, we will only load 1 page, not 2
    {
        //    puts("@@@@@ we're fixing the victimized pcb @@@@@");
        char str[40];
        sprintf(str, "./BackingStore/%d.txt", readyQueue.head->PID);
        FILE *fileInBackingStore = fopen(str, "r");
        // we know that since it's been victimized, findFrame() will always return -1
        // then we can right away execute the code from memorymanager.c to handle victim frames
        int victimFrame = findVictim(readyQueue.head);
        loadPage(readyQueue.head->PC_page, fileInBackingStore, victimFrame);
        readyQueue.head->pageTable[readyQueue.head->PC_page] = victimFrame;
        updatePageTable(readyQueue.head, readyQueue.head->PC_page, victimFrame, victimFrame);
        readyQueue.head->PC_page++;
        CPU.IP = (readyQueue.head)->pageTable[readyQueue.head->PC_page - 1]; // CPU.IP holds the frame #
    }

    CPU.offset = (readyQueue.head)->PC_offset;

    int condition = run(2);
    if (condition == 1)
    {
        terminate();
        return;
    }

    if (condition == 2)
    { // this is when we changed pages in run, so we don't want to increment PC and PC_offset
        roundRobin();
        return;
    }
    // ELSE
    readyQueue.head->PC += 2;
    readyQueue.head->PC_offset += 2;
    roundRobin();
    return;

}


// this will always do the same, i.e. place head in the back of queue and move other programs up 
void roundRobin()
{
    struct PCB *tmp = readyQueue.head;
    if (tmp->next == NULL)
    {
        scheduler(); // i.e. only one program in ready queue
    } else if (tmp->next == readyQueue.tail) // i.e. only two programs in ready queue
    {
        readyQueue.head = readyQueue.tail;
        readyQueue.tail = tmp;
        readyQueue.head->next = readyQueue.tail;
        readyQueue.tail->next = NULL;
        scheduler();
    } else
    {

        readyQueue.head = tmp->next;
        tmp->next = NULL;
        (readyQueue.tail)->next = tmp;
        readyQueue.tail = tmp;
        scheduler();
    }
}

// this will terminate the program, this will always be called when the head of readyQueue has finished
void terminate()
{
    if ((readyQueue.head)->next == NULL) // i.e. only one program left
    {
        int pid = readyQueue.head->PID;
        char str[40];
        sprintf(str, "rm BackingStore/%d.txt", pid);
        system(str); // removes the file when done executing
        free(readyQueue.head);
        readyQueue.head = NULL;
        return; // doesnt call scheduler, returns back to interpreter();
    }
    int pid = readyQueue.head->PID;
    char str[40];
    //printf("the PID that we're removing is : %d \n", pid);
    sprintf(str, "rm BackingStore/%d.txt", pid);
    system(str); // removes the file when done executing
    struct PCB *tmp = (readyQueue.head)->next;
    free(readyQueue.head);
    readyQueue.head = tmp;
    scheduler();
}