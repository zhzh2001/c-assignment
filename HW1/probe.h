#ifndef __PROBE_H
#define __PROBE_H
// prevent loop including

static int dx[] = {-1, 1, 0, 0}, dy[] = {0, 0, -1, 1}; // dx: row increment of 4 directions, dy: col
void findStartEnd();
int probe(int r, int c);
#endif