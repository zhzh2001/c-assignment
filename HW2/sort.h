#ifndef __SORT_H
#define __SORT_H
#include "general.h"
listNode_t *selectionSort(listNode_t *head);
listNode_t *bubbleSort(listNode_t *head);
listNode_t *insertionSort(listNode_t *head);
listNode_t *selectLikeSort(listNode_t *head);
listNode_t *mergeSort(listNode_t *head);
listNode_t *copyQSort(listNode_t *head);
typedef struct sort
{
	char *name;								   // name of the sorting algorithm
	listNode_t *(*function)(listNode_t *head); // actual function
} sort_t;									   // store sorting algorithms
// convenient when adding new algorithm & reduce duplicated calls with loop
#endif