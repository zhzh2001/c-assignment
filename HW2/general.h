#ifndef __GENERAL_H
#define __GENERAL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct listNode
{
	int value;
	struct listNode *next;
} listNode_t; // linked list
#endif