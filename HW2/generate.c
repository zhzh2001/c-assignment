#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main()
{
	freopen("input", "w", stdout);
	int n, m;
	scanf("%d%d", &n, &m);
	for (int i = 0; i < n; i++)
		printf("%d ", rand() % m);
	puts("");
	return 0;
}