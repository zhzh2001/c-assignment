#ifndef __LIST_H
#define __LIST_H
#include "general.h"
listNode_t *createList();
void swapNode(listNode_t *a, listNode_t *b);
void printList(listNode_t *head);
listNode_t *copyList(listNode_t *head);
void freeList(listNode_t *head);
#endif