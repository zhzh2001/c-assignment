#include "maze.h"
#include "probe.h"

/**
 * find starting & ending point
**/
void findStartEnd()
{
	for (int j = 1;; j++)
		if (maze.tile[1][j] == tileEmpty) // find the first empty tile on the second row as start
		{
			maze.tile[1][maze.startC = j] = tileStart; // record startC
			break;
		}
	for (int j = maze.col - 2;; j--)
		if (maze.tile[maze.row - 2][j] == tileEmpty) // find the last empty tile on the second to last row as end
		{
			maze.tile[maze.row - 2][j] = tileEnd; // no need to record endC, as probe would find it
			break;
		}
}

/**
 * probe tile @ [r][c]
 * @ret success = 1, failure = 0
**/
int probe(int r, int c)
{
	for (int i = 0; i < 4; i++)
	{
		int newr = r + dx[i], newc = c + dy[i]; // new tile to explore
		if (maze.tile[newr][newc] == tileEnd)	// reach the end!
			return 1;
		if (maze.tile[newr][newc] == tileEmpty) // no need to worry about out of bounds, as walls surround the maze
		{
			maze.tile[newr][newc] = tilePath; // mark as solution path
			if (probe(newr, newc))			  // reach the end
				return 1;
			maze.tile[newr][newc] = tileEmpty; // backtracking
		}
	}
	return 0;
}