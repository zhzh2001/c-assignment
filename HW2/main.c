#include "general.h"
#include "list.h"
#include "sort.h"

/**
 * @argv[1] optional redirect stdin for large size testing
 * @argv[2] optional redirect stdout
**/
int main(int argc, char *argv[])
{
	extern sort_t sortAlgos[]; // use extern to reference variable in sort.c
	extern int totalAlgos;
	for (int i = 0; i < totalAlgos; i++)
		printf("%d: %20s\n", i, sortAlgos[i].name);
	int sortID[totalAlgos]; // specify id of algorithms to test
	for (int i = 0; i < totalAlgos; i++)
		if (scanf("%d", sortID + i) != 1)
		{
			totalAlgos = i; // less than count of algorithms
			break;
		}

	if (argc > 1) // process arguments
	{
		if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
		{
			puts("usage: sort [input] [output]"); // display help
			return 0;
		}
		freopen(argv[1], "r", stdin);
		if (argc > 2)
			freopen(argv[2], "w", stdout);
	}

	listNode_t *origin = createList();
	for (int i = 0; i < totalAlgos; i++)
	{
		listNode_t *head = copyList(origin); // copy to preserve origin
		struct timespec start, stop;
		clock_gettime(CLOCK_MONOTONIC, &start);		   // start timer
		head = (*sortAlgos[sortID[i]].function)(head); // call relevant function
		clock_gettime(CLOCK_MONOTONIC, &stop);		   // stop timer
		printList(head);
		fprintf(stderr, "%20s time = %.9lf s\n", sortAlgos[sortID[i]].name,
				(stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) * 1e-9);
		// print to terminal even when I/O redirected
		freeList(head);
	}
	freeList(origin);
}