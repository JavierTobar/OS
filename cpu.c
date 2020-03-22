#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "ram.h"
#include "interpreter.h"
struct CPU
{
	int IP;
	char IR[1000];
	int quanta;
} CPU;

int run(int quanta)
{
	while (quanta > 0)
	{
		strcpy(CPU.IR, ram[CPU.IP]);
		if (strncmp(CPU.IR, "quit", 4) == 0) // if quit, script terminates, but not program
		{
			return 1; // this will let scheduler know to call terminate on this script
		} else {
		interpret(CPU.IR);
		quanta--;
		CPU.IP++;
		}
	}
	CPU.quanta = 2; // reset for next call
	return 0;
}
