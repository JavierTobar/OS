#ifndef CPU_H
#define CPU_H

/*
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
} CPU;

/*
	This function will execute the process at the head of the readyQueue for x Quanta
	Note : Our OS assumes that every line of code is 1 quanta
	In reality, this is much more complex  
*/
int run(int quanta);

#endif
