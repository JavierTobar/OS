#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H
#include <stdio.h>

extern char *fileName;

/**
 *
 * @param pageNumber the wanted page from BackingStore
 * @param f file pointer
 * @param frameNumber frame number to load page into
 */
void loadPage(int pageNumber, FILE *f, int frameNumber);

/**
 * FIFO Search to find empty frame in RAM
 * @return frame index or -1 if RAM full
 */
int findFrame();

/**
 * Finds victim frame
 *
 * @param p pcb that we want to load
 * @return victim frame index
 */
int findVictim(struct PCB *p);

/**
 * Updates page table of the given pcb
 *
 * @param p pcb that we want to update
 * @param pageNumber    page number
 * @param frameNumber   frame number
 * @param victimFrame   victimFrame
 * @return error code
 */
int updatePageTable(struct PCB *p, int pageNumber, int frameNumber, int victimFrame);

/**
 * Loads two pages from the file pointer into BackingStore
 *
 * @param p file pointer
 * @return error code
 */
int launcher(FILE *p);

#endif