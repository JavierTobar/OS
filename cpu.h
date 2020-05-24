#ifndef CPU_H
#define CPU_H

/**
    Our CPU struct has 3 fields
	IP is our instruction pointer
	IR is our (simulated) instruction register
	quanta is how long our CPU will run for
 */
struct CPU
{
	int IP;
	char IR[1000];
	int quanta;
	int offset;
} CPU;

/**
 * Runs the head of the readyQueue for # quanta
 *
 * @param quanta always 2 now
 * @return error code
 */
int run(int quanta);

#endif