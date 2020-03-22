#ifndef RAM_H
#define RAM_H
#include <stdio.h>

/*
  Our ram is simulated by an array
*/
extern char *ram[1000];

/*
  This function copies a file into (simulated) RAM
*/
void addToRAM(FILE *p, int *start, int *end);

#endif