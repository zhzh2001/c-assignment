#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int main()
{
	struct timespec start, stop;
	clock_gettime(CLOCK_MONOTONIC, &start);
	int i;
	for (i = 1;; i++)
	{
		clock_gettime(CLOCK_MONOTONIC, &stop);
		if (stop.tv_nsec != start.tv_nsec)
			break;
	}
	printf("%ld ns %d loop\n", stop.tv_nsec - start.tv_nsec, i);
	return 0;
}