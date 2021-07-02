#ifndef __MAZE_H
#define __MAZE_H
// prevent loop including

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// basic headers

struct maze_t
{
	int row, col; // row & col of maze
	int startC;	  // column of starting point
	char **tile;  // dynamic alloc 2d array
} maze;

enum tile_t
{
	tileEmpty = ' ', // empty tile
	tileWall = '#',	 // wall (traditional), actually display U+258C (left half block)
	tilePath = '*',	 // the solution path
	tileStart = 'S', // starting point
	tileEnd = 'E'	 // end
};

int fallback; // whether to fallback to traditional mode of wall

#endif