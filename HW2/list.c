#include "general.h"
#include "list.h"

/**
 * create a linked list from stdin
 * end with EOF
**/
listNode_t *createList()
{
	listNode_t vhead, *p = &vhead; // use virtual head to simplify code
	for (;;)
	{
		int value;
		if (scanf("%d", &value) != 1)
		{
			p->next = NULL; // list ending
			break;
		}
		listNode_t *q = (listNode_t *)malloc(sizeof(listNode_t));
		q->value = value;
		p->next = q; // p is previous tail; work with virtual head, too
		p = q;
	}
	return vhead.next; // eliminate virtual head
}

/**
 * swap value of two nodes
**/
void swapNode(listNode_t *a, listNode_t *b)
{
	int t = a->value;
	a->value = b->value;
	b->value = t;
}

/**
 * print a list to stdout
 * including line break
**/
void printList(listNode_t *head)
{
	for (listNode_t *p = head; p; p = p->next)
		printf("%d ", p->value);
	puts("");
}

/**
 * hard copy a list
 * resemble createList
**/
listNode_t *copyList(listNode_t *head)
{
	listNode_t vhead, *p = &vhead, *pp = head; // pp is like stdin in createList
	for (;;)
	{
		if (!pp) // end of list
		{
			p->next = NULL;
			break;
		}
		listNode_t *q = (listNode_t *)malloc(sizeof(listNode_t));
		q->value = pp->value;
		pp = pp->next; // next node
		p->next = q;
		p = q;
	}
	return vhead.next;
}

/**
 * free all nodes of a list
 * call it when destroying a list
**/
void freeList(listNode_t *head)
{
	listNode_t vhead, *p = &vhead;
	p->next = head;
	while (p->next)
	{
		listNode_t *q = p->next;
		p->next = q->next; // remove all nodes
		free(q);
	}
}