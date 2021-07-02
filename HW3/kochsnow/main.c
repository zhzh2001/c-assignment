#include <stdio.h>
#include <math.h>
#include <stdlib.h> 
#include "extgraph.h"
#include <windows.h>

#define TitleMargin 0.5
#define TitleSize 16

double pi = acos(-1);

void DrawPolarLine(double r, double theta)
{
	double radians = theta / 180 * pi;
	DrawLine(r * cos(radians), r * sin(radians));
}

void Main()
{
	InitConsole();
	printf("Enter n: ");
	int n;
	scanf("%d", &n);
	printf("Enter len: ");
	double len;
	scanf("%lf", &len);
	PostMessage(GetConsoleWindow(), WM_QUIT, 0, 0);
	
	// Draw Title
	SetWindowTitle("Draw A Kochsnow");
	InitGraphics();
	double width = GetWindowWidth();
	double height = GetWindowHeight();
	MovePen(TitleMargin, height - TitleMargin);
	SetPointSize(TitleSize);
	DrawTextString("Let's draw a kochsnow without recursion!");
	
	int cnt = 3;
	int *pre = malloc(cnt * sizeof(int));
	pre[0] = 0; pre[1] = 120; pre[2] = 240;
	for (int i = 1; i <= n; i++)
	{
		int ncnt = cnt * 4;
		int *now = malloc(ncnt * sizeof(int));
		for (int j = 0; j < cnt; j++)
		{
			now[j * 4] = pre[j];
			now[j * 4 + 1] = pre[j] - 60;
			now[j * 4 + 2] = pre[j] + 60;
			now[j * 4 + 3] = pre[j];
		}
		free(pre);
		cnt = ncnt;
		pre = now;
	}
	
	MovePen(width / 2 - len / 2,
			(height - TitleMargin) / 2 - len / 2 * tan(pi / 6));
	len /= pow(3, n);
	for (int i = 0; i < cnt; i++)
		DrawPolarLine(len, pre[i]);
}
