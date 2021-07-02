#include <stdio.h>
#include "extgraph.h"

#define TitleMargin 0.5
#define TitleSize 16

double px, py, width, height;
bool down;

void myMouseEvent(int x, int y, int button, int event)
{
	double nx = ScaleXInches(x), ny = ScaleYInches(y);
	switch (event)
	{
	case MOUSEMOVE:
		if (down)
			DrawLine(nx - px, ny - py);
		else
			MovePen(nx, ny);
		px = nx; py = ny;
		break;
	case BUTTON_DOWN:
		down = TRUE;
		break;
	case BUTTON_UP:
		down = FALSE;
		break;
	}
}

void Main()
{
	// Draw Title
	SetWindowTitle("Draw With Mouse");
	InitGraphics();
	width = GetWindowWidth();
	height = GetWindowHeight();
	MovePen(TitleMargin, height - TitleMargin);
	SetPointSize(TitleSize);
	DrawTextString("Let's draw with mouse!");
	registerMouseEvent(myMouseEvent);
}
