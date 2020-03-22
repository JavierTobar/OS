#ifndef RAM_H
#define RAM_H
#include <stdio.h>

extern char *ram[1000];

void addToRAM(FILE *p, int *start, int *end);

#endif
