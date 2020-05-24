#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "kernel.h"
#include "ram.h"
#include "pcb.h"

void loadPage(int pageNumber, FILE *f, int frameNumber);

int findFrame();

int findVictim(struct PCB *p);

int countTotalPages(FILE *f);

int updatePageTable(struct PCB *p, int pageNumber, int frameNumber, int victimFrame);

char *fileName;

int launcher(FILE *p)
{
    struct PCB *pcb = myinit(fileName);
    int pid = pcb->PID;
    int counter = 0;

    char str[40], c; // this will hold the path to our file
    sprintf(str, "./BackingStore/%d.txt", pid);

    FILE *fileInBackingStore = fopen(str, "w"); // creates the file and opens it in write mode

    if (fileInBackingStore == NULL)
    {
        printf("Cannot find the directory BackingStore \n");
        return 0; // i.e. error
    }
    // copy contents of p into fileInBackingStore
    while ((c = fgetc(p)) != EOF)
    {
        fputc(c, fileInBackingStore);
    }
    fclose(p);
    //rewind(fileInBackingStore); // point back to start of file since fputc advances pointer
    fclose(fileInBackingStore);
    fileInBackingStore = fopen(str, "r");
    int totalPages = countTotalPages(fileInBackingStore);
    pcb->pages_max = totalPages;
    rewind(fileInBackingStore); // might be problem

    // MAIN LOGIC OF THE FUNCTION
    while (counter < 2) // we only load 2 pages max
    {


        if (pcb->PC_page != (pcb->pages_max - 1)) // since PC_page starts at 0 but pages_max starts at 1
        {
            counter++;
        } else
        {
            counter = 2; // i.e. PC_page = pages_max-1 means that we're loading our last page
        }

        int frame = findFrame();

        if (frame != -1) // i.e. we found a frame
        {
            loadPage(pcb->PC_page, fileInBackingStore, frame); // we load the page
            pcb->pageTable[pcb->PC_page] = frame;
            updatePageTable(pcb, pcb->PC_page, frame, -1);
            pcb->PC_page++;
        } else // we have a victimFrame
        {
            int victimFrame = findVictim(pcb);
            loadPage(pcb->PC_page, fileInBackingStore, victimFrame);
            pcb->pageTable[pcb->PC_page] = victimFrame;
            updatePageTable(pcb, pcb->PC_page, victimFrame, victimFrame);
            pcb->PC_page++;
        }
    }
    pcb->PC_page = 0;
    fclose(fileInBackingStore);
    return 1;
}

int countTotalPages(FILE *f)
{
    int counter = 0;
    char c;
    c = getc(f);
    while (c != EOF)
    {
        if (c == '\n')
        {
            counter++;
        }
        c = fgetc(f);

    }
    if (counter % 4 == 0)
    {
        return counter / 4;
    }
    // ELSE
    return counter / 4 + 1; // this returns the TOTAL pages
}

void loadPage(int pageNumber, FILE *f, int frameNumber)
{
    int counter = 0; // counts the lines
    char line[256], c;

    // this while loop is to count the lines
    c = getc(f);
    rewind(f); // to prevent the char eating in the previous line from affecting fgets

    while (c != EOF && counter != pageNumber * 4) // i.e. our pages start at index 0
    {
        c = fgetc(f);
        if (c == '\n')
        {
            counter++;
        }
    }
    // now our file pointer is pointing to the proper location
    // we will now load 1 page, i.e. 4 lines of code (or less) from the page into the frame in ram[]
    counter = 0;
    while (fgets(line, sizeof(line), f) && counter < 4) // we only load 1 page, i.e. 4 lines of code
    {

        ram[frameNumber * 4 + counter] = strdup(line); // this also overwrites if we had something there previously
        counter++;
    }
    rewind(f);
}

int findFrame()
{
    int index = 0;
    while (index <= 9)
    {
        if (ram[index * 4] == NULL)
        {
            return index;
        }
        index++;
    }

    // this will be reached if we didn't find an empty frame
    return -1;
}

// if findFrame() returns -1 then we need to victimize a frame
int findVictim(struct PCB *p)
{
    int index = rand() % 10; // returns pseudo-random victim index between 0 and 9
    int i;
    while (1)
    {
        for (i = 0; i < 10; i++)
        {
            if (p->pageTable[i] == index) // i.e. the frame BELONGS to the pages of the active PCB
                // there could be an infinite loop here, but our program won't be
                // big enough to utilize all 10 frames
            {
                index = (index + 1) % 10; // we increment the index
                i = 0; // we reset our i to 0 and look again
            }
        }
        // we reach this if the frame (index) is not linked with any pages of the active PCB
        return index; // our exit condition is to simply return the index
    }
}

/**
 * HELPER METHOD
 * @param readyQueue : contains all our PCBs
 * @param victimFrame : the frame that we got from findVictim(PCB *p)
 * @return PCB that had the frame which is being removed in updatePageTable
 */
struct PCB *findVictimPCB(struct ReadyQueue rdyQ, int victimFrame)
{
    // we need to iterate through all the PCB's
    // we need to iterate through each PCB's pageTable to see if they hold the victimFrame
    struct PCB *tmp = readyQueue.head;
    while (tmp != NULL)
    {
        for (int i = 0; i < 10; i++)
        {
            if (tmp->pageTable[i] == victimFrame) // i.e this PCB was using the victimFrame
            {
                return tmp;
            }
        }
        // if this PCB didn't have the victimFrame then keep looking through the readyList
        tmp = tmp->next;
    }
    return NULL; // if we return NULL then this means that the victimFrame wasn't being used by any PCBs
    // ideally this shouldn't happen since this method will only get called when we know we had a victimFrame
}


int findPageNumberOfVictimPCB(struct PCB *p, int victimFrame)
{
    for (int i = 0; i < 10; i++)
    {
        if (p->pageTable[i] == victimFrame)
        {
            return i;
        }
    }
    return -1; // something went wrong if we return -1
}




int updatePageTable(struct PCB *p, int pageNumber, int frameNumber, int victimFrame)
{
    if (victimFrame >= 0 && pageNumber == -1) // if victimFrame >= 0 then this is when we're taking care of the
        // PCB victim. i.e. we remove it's previous frame
    {
        int victimPageNumber = findPageNumberOfVictimPCB(p, victimFrame);
        if (victimPageNumber == -1)
        {
            printf("Error: something went wrong trying to find the page number of victim PCB \n");
            return -1; // error code
        }
        p->pageTable[victimPageNumber] = -1;
        return 0;
    }

    if (victimFrame == -1) // if victimFrame = -1 then we didn't have a victimFrame
        // i.e. findFrame() did NOT return -1
    {
        p->pageTable[pageNumber] = frameNumber;
        return 0; // 0 means no error and it's completed successfully
    }
    // ELSE THERE IS A VICTIM so we need to take care of the victim
    struct PCB *victim = findVictimPCB(readyQueue, victimFrame);

    // in theory the following if statement should NEVER happen since findVictim is only called when
    // there is a victim, however this is here for debugging purposes
    if (victim == NULL)
    {
        printf("Error: couldn't find the victim PCB \n");
        return -1; //error code
    }

    // design decision: passing -1 as pageNumber let's the method know that
    // we're taking care of the victim and thus need to find its own pageNumber
    int err = updatePageTable(victim, -1, frameNumber, victimFrame);

    // again, i don't see any way how this if statement can be reached but it's here for
    // debugging purposes or if something goes really wrong
    if (err != 0)
    {
        printf("Error: couldn't clear the victim PCB pageTable \n");
        return -1;
    }
    // once the victim PCB has been taken care of we can finish updating the PCB we initially wanted to update
    p->pageTable[pageNumber] = victimFrame;
}