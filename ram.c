#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "ram.h"

char *ram[40];


/**
 * FUNCTION IS DEPRECATED
 * REPLACED BY MEMORYMANAGER'S LOADPAGE()
 */
//void addToRAM(FILE *p, int *start, int *end)
//{
//	int i;
//	char buffer[100]; // assumes max of 100 char per line
//	for (i = 0; i < 40; i++)
//	{
//		if (ram[i] == NULL) break; // this is to find the first null cell
//	}
//	if (i == 999)
//	{
//		printf("No space in ram for the file \n");
//		return;
//	} else
//		{
//
//			*start = i;
//			while(fgets(buffer, 1000, p) != NULL)
//			{
//				if (i == 999)
//				{
//					printf("No space in ram for the file \n");
//					return;
//				}
//				ram[i] = strdup(buffer);
//				i++;
//			}
//			i--;
//			*end = i;
//		}
//}