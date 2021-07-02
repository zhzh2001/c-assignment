#include "maze.h"
#include "init.h"

/**
 * allocate memory for maze & create dynamic 2d array
 * @row row of maze
 * @col column of maze
 * @ret error = 1, success = 0
**/
int mazeAlloc(int row, int col)
{
	if (buf)
		return 1;									// already allocated
	buf = (char *)malloc(sizeof(char) * row * col); // allocate actual tiles
	if (!buf)
		return 1; // malloc fail

	maze.row = row;
	maze.col = col;
	maze.tile = (char **)malloc(sizeof(char *) * row); // allocate headers
	if (!maze.tile)
		return 1; // malloc fail

	for (int i = 0; i < row; i++)
		maze.tile[i] = buf + i * col; // init headers
	return 0;						  // success
}

/**
 * prompt user to enter maze size
**/
void getSize()
{
	int row, col;
	printf("Enter row of maze: ");
	scanf("%d", &row);
	printf("Enter column of maze: ");
	scanf("%d", &col);
	if (row < 5) // for robustness and convenience (for findStartEnd)
	{
		puts("Too few rows, adjust to 5");
		row = 5;
	}
	if (col < 5)
	{
		puts("Too few columns, adjust to 5");
		col = 5;
	}

	if (mazeAlloc(row, col)) // allocate fail
	{
		puts("Fatal error: failed to allocate memory for maze");
		exit(1);
	}
}

/**
 * create the graph for Kruskal
 * example pre-maze 7 x 7:
 * #######
 * #.H.H.#
 * #V#V#V#
 * #.H.H.#
 * #V#V#V#
 * #.H.H.#
 * #######
 * row = col = 7 / 2 = 3 (every 2 row / col)
 * empty tiles (.) = 3 * 3 => vertices of graph
 * vertical edges (V) = 2 * 3
 * horizonal edges (H) = 3 * 2
**/
void createGraph()
{
	int row = maze.row / 2, col = maze.col / 2, n = row * col; // vertices of graph
	fat = (int *)malloc(sizeof(int) * n);					   // allocate for disjoint set
	if (!fat)
	{
		puts("Fatal error: failed to allocate memory for disjoint set");
		exit(1);
	}
	for (int i = 0; i < n; i++)
		fat[i] = i; // init disjoint set

	int m = (row - 1) * col + row * (col - 1);	  // number of edges
	edges = (edge_t *)malloc(sizeof(edge_t) * m); // allocate for edges
	if (!edges)
	{
		puts("Fatal error: failed to allocate memory for edges");
		exit(1);
	}

	int cnt = 0; // counter for edges
	for (int i = 2; i < maze.row - 1; i += 2)
		for (int j = 1; j < maze.col - 1; j += 2)
		{
			edges[cnt].dir = 0; // vertical edge
			edges[cnt].r = i;
			edges[cnt++].c = j; // increase counter
		}
	for (int i = 1; i < maze.row - 1; i += 2)
		for (int j = 2; j < maze.col - 1; j += 2)
		{
			edges[cnt].dir = 1; // horizonal edge
			edges[cnt].r = i;
			edges[cnt++].c = j;
		}

	srand(time(NULL)); // init random number generator
	// random shuffle edges
	for (int i = 0; i < m * 2; i++)
	{
		int x = randn(m), y = randn(m);
		edge_t tmp = edges[x]; // swap edges
		edges[x] = edges[y];
		edges[y] = tmp;
	}
}

/**
 * use Kruskal's MST algorithm to generate the maze
 * basically just randomly connect tiles to create pathway
**/
void Kruskal()
{
	memset(buf, tileWall, sizeof(char) * maze.row * maze.col); // set all tiles to wall
	int row = maze.row / 2, col = maze.col / 2, n = row * col; // vertices of graph
	for (int i = 1; i < maze.row - 1; i += 2)
		for (int j = 1; j < maze.col - 1; j += 2)
			maze.tile[i][j] = tileEmpty;	   // init n empty tiles
	int m = (row - 1) * col + row * (col - 1); // number of edges

	for (int i = 0; i < m; i++)
	{
		int u, v;
		if (edges[i].dir == 0) // vertical edge!
		{
			// transform to 1d array: r * col + c, here r & c should be half for storage
			u = (edges[i].r - 1) / 2 * col + edges[i].c / 2; // tile [r - 1][c]
			v = (edges[i].r + 1) / 2 * col + edges[i].c / 2; // tile [r + 1][c]
		}
		else
		{
			u = edges[i].r / 2 * col + (edges[i].c - 1) / 2; // tile [r][c - 1]
			v = edges[i].r / 2 * col + (edges[i].c + 1) / 2; // tile [r][c + 1]
		}
		int ru = getfat(u), rv = getfat(v); // find ancestor
		if (ru != rv)						// not in the same tree
		{
			fat[ru] = rv;								   // merge
			maze.tile[edges[i].r][edges[i].c] = tileEmpty; // add a tile as edge to connect
			if ((--n) == 1)								   // n - 1 vertices are all connected, done
				break;
		}
	}
}

/**
 * disjoint set core algorithm
 * @v vertex to find ancestor
 * @ret its ancestor
**/
int getfat(int v)
{
	return fat[v] == v ? v : (fat[v] = getfat(fat[v])); // apply path compression for performance
}

/**
 * generate random int in [0, n)
 * why not just `rand() % n`?
 * platform problem: under Windows rand() is up to 0x7fff or 32767
 * so if n > 0x7fff, randn wouldn't work uniformly
**/
int randn(int n)
{
	return ((rand() & 0x7fff) << 15 | (rand() & 0x7fff)) % n; // n up to 2 ^ 30
}

/**
 * to display Unicode properly under Windows, have to alter code page for command prompt
 * under other platforms, simply do nothing
 * then test to see if need to fallback
**/
void initGraph()
{
#ifdef _WIN32
	//universal way to detect Windows platform
	system("chcp 65001"); // change code page to 65001 (UTF-8)
#endif
	printf("Is the following character \xe2\x96\x8c a block? (Y / N) ");
	char ch;
	scanf("%c", &ch);
	if (ch == 'n' || ch == 'N') // user cannot see the block character, fallback to '#'
	{
		puts("Fallback to traditional mode of displaying wall");
		fallback = 1;
	}
}