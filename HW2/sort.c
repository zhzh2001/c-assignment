#include "general.h"
#include "sort.h"
#include "list.h"

sort_t sortAlgos[] = {"Selection Sort", selectionSort,
					  "Bubble Sort", bubbleSort,
					  "Insertion Sort", insertionSort,
					  "Selection-like Sort", selectLikeSort,
					  "Merge Sort", mergeSort,
					  "Copied Quick Sort", copyQSort}; // omit {} of struct
int totalAlgos = sizeof(sortAlgos) / sizeof(sort_t);   // number of algorithms present

/**
 * perform in-place selection sort
**/
listNode_t *selectionSort(listNode_t *head)
{
	for (listNode_t *p = head; p; p = p->next)
	{
		listNode_t *min = p;
		for (listNode_t *q = p->next; q; q = q->next)
			if (q->value < min->value)
				min = q;  // select the node with minimal value
		swapNode(p, min); // swap with current node
	}
	return head; // useless, only for consistence with insertionSort
}

/**
 * perform in-place bubble sort
**/
listNode_t *bubbleSort(listNode_t *head)
{
	listNode_t *last = NULL; // where the "bubble" last run reaches (sorted sequence)
	while (head != last)	 // stop when sorted sequence is the whole list
	{
		listNode_t *p, *q;
		for (p = head; p->next != last; p = q) // stop when reaching sorted sequence
		{
			q = p->next;
			if (p->value > q->value)
				swapNode(p, q); // "bubble" goes to tail
		}
		last = p;
	}
	return head;
}

/**
 * perform in-place insertion sort
 * with changes in linked list structure (dangerous!)
 * seem ugly & might be *buggy*
**/
listNode_t *insertionSort(listNode_t *head)
{
	listNode_t vhead = {0, head};			  // use virtual head as actual head could change
	for (listNode_t *p = head; p && p->next;) // ensure p->next is not NULL
	{
		listNode_t *now = p->next, *q; // head..p: sorted sequence; now: the element to insert
		for (q = &vhead; q->next != now && q->next->value < now->value; q = q->next)
			;				// find the insertion position
		if (q->next != now) // need insertion
		{
			p->next = now->next; // remove now
			now->next = q->next;
			q->next = now; // insert now after q
		}
		else
			p = p->next; // move p only when no insertion occurs
	}
	return vhead.next;
}

/**
 * perform simple selection/bubble-like sort
 * commonly used by OIers
**/
listNode_t *selectLikeSort(listNode_t *head)
{
	for (listNode_t *p = head; p; p = p->next)
		for (listNode_t *q = p->next; q; q = q->next)
			if (p->value > q->value)
				swapNode(p, q);
	return head;
}

/**
 * in-place merge two sorted list
**/
static listNode_t *merge(listNode_t *head1, listNode_t *head2)
{
	listNode_t vhead, *p = &vhead; // virtual head
	while (head1 && head2)
	{
		if (head1->value < head2->value) // link to head1
		{
			p->next = head1;
			head1 = head1->next;
		}
		else // link to head2
		{
			p->next = head2;
			head2 = head2->next;
		}
		p = p->next;
	}
	if (head1) // link to remaining nodes
		p->next = head1;
	if (head2)
		p->next = head2;
	return vhead.next;
}

/**
 * perform in-place merge sort
 * time complexity O(N log N), better than above
 * reference: https://www.geeksforgeeks.org/merge-sort-for-linked-list/
**/
listNode_t *mergeSort(listNode_t *head)
{
	if (!head || !head->next)
		return head;
	// use The Tortoise and The Hare Approach to find midpoint
	listNode_t *slow = head, *fast = head->next;
	while (fast && fast->next)
	{
		slow = slow->next;
		fast = fast->next->next; // go two steps every time
	}
	listNode_t *head2 = slow->next;
	slow->next = NULL; // unlink to separate two lists
	listNode_t *newHead1 = mergeSort(head);
	listNode_t *newHead2 = mergeSort(head2);
	return merge(newHead1, newHead2); // divide and conquer
}

/**
 * cmp for qsort
**/
static int cmp(const void *a, const void *b)
{
	return *(int *)a - *(int *)b;
}

/**
 * copy list to array to perform qsort from stdlib
**/
listNode_t *copyQSort(listNode_t *head)
{
	int count = 0;
	listNode_t *p;
	for (p = head; p; p = p->next)
		count++;
	int *arr = (int *)malloc(count * sizeof(int));
	int i = 0;
	for (p = head; p; p = p->next)
		arr[i++] = p->value;
	qsort(arr, count, sizeof(int), cmp);
	i = 0;
	for (p = head; p; p = p->next)
		p->value = arr[i++];
	return head;
}