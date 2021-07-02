#ifndef __INIT_H
#define __INIT_H
// prevent loop including
static char *buf; // actual tiles of maze
static int *fat;  // disjoint set
struct edge		  // an intermediate tile as an edge
{
	int r, c, dir; // dir: vertical = 0, horizonal = 1
};
typedef struct edge edge_t;
static edge_t *edges; // all edges for Kruskal
int mazeAlloc(int row, int col);
void getSize();
void createGraph();
int getfat(int v);
void Kruskal();
int randn(int n);
void initGraph();
#endif