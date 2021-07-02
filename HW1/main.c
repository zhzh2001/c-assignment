#include "maze.h"
#include "init.h"
#include "print.h"
#include "probe.h"

/**
 * main function
 * @argc number of arguments
 * @argv vector of arguments
**/
int main(int argc, char *argv[])
{
	initGraph();
	getSize();
	createGraph();
	Kruskal();
	findStartEnd();
	puts("Here is the maze:");
	printMaze();
	probe(1, maze.startC);
	puts("Here is the maze with solution:");
	printMaze();
	return 0;
}