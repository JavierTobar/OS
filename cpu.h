#ifndef CPU_H
#define CPU_H

struct CPU
{
	int IP;
	char IR[1000];
	int quanta;
} CPU;

int run(int quanta);

#endif 
