#include <stdio.h>
#include "extgraph.h"
#include "angular.h"

#define TitleMargin 0.5
#define TitleSize 16
#define InnerDeg 20
#define InnerLen 1.2
#define DiamondDeg 60
#define OuterLen (InnerLen * 2)

void Main()
{
	// Draw Title
	SetWindowTitle("Draw A Beatiful Shape");
	InitGraphics();
	double width = GetWindowWidth();
	double height = GetWindowHeight();
	MovePen(TitleMargin, height - TitleMargin);
	SetPointSize(TitleSize);
	DrawTextString("Let's draw a beatiful shape with \
					the help of angular.h");
	char buf[50];
	sprintf(buf, "Degree = %d Length = %.2lf (adjustable from source code)",
			InnerDeg, InnerLen);
	MovePen(TitleMargin, height - TitleMargin - GetFontHeight());
	DrawTextString(buf);
	
	// Draw Inner Diamonds
	MovePen(width / 2, height / 2);
	turn(InnerDeg / 2);
	for (int i = 1; i <= 360 / InnerDeg; i++)
	{
		forward(InnerLen);
		turn(DiamondDeg);
		forward(InnerLen);
		turn(DiamondDeg * 2);
		forward(InnerLen);
		turn(DiamondDeg);
		forward(InnerLen);
		turn(DiamondDeg * 2);
		turn(InnerDeg);
	}
	
	// Draw Outer Hexagons
	for (int i = 1; i <= DiamondDeg / InnerDeg; i++)
	{
		MovePen(width / 2, height / 2);
		move(OuterLen);
		turn(DiamondDeg * 2);
		for (int j = 1; j <= 6; j++)
		{
			forward(OuterLen);
			turn(DiamondDeg);
		}
		turn(InnerDeg);
	}
}
