#ifndef PCB_H
#define PCB_H

/*
	Our PCB struct has :
	a PC : Program Counter
	a start : index in mememory
	an end : index in memory
	a pointer : to the PCB that comes after the current PCB 
	This pointer is crucial for roundRobin scheduling
*/
struct PCB
{
	int PC;
	int start;
	int end;
	struct PCB *next;
};

/*
	This function malloc's a PCB and initializes it accordingly
*/
struct PCB* makePCB(int start, int end);

#endif
