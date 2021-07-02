#include "maze.h"
#include "print.h"

/**
 * print the maze
**/
void printMaze()
{
	printf("Size of maze: %d x %d\n", maze.row, maze.col); // output size for convenience
	for (int i = 0; i < maze.row; i++)
	{
		for (int j = 0; j < maze.col; j++)
			if (maze.tile[i][j] == tileWall && !fallback) // not fallback to '#'
				printf("\xe2\x96\x8c");					  // Unicode U+258C (left half block), great visual effect
			else
				putchar(maze.tile[i][j]);
		puts(""); // newline
	}
}