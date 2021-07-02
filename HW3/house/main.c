#include <stdio.h>
#include "extgraph.h"

#define TitleMargin 0.5
#define TitleSize 16
#define HouseMargin 1.5
#define HouseWidth 2.0
#define RoofHeight HouseWidth
#define ChimneyLeft (HouseWidth * 0.1)
#define ChimneyWidth (HouseWidth * 0.12)
#define ChimneyDelta (HouseWidth * 0.05)
#define ChimneyHeight (RoofHeight * 0.4)
#define BoxWidth (ChimneyDelta * 2 + ChimneyWidth)
#define BoxHeight (RoofHeight * 0.1)
#define BodyHeight (RoofHeight * 1.2)
#define Spacing (HouseWidth * 0.1)
#define WindowWidth (HouseWidth * 0.2)
#define WindowHeight (BodyHeight * 0.2)
#define WindowTop (BodyHeight * 0.2)
#define DoorWidth WindowWidth
#define DoorHeight (BodyHeight - WindowTop)

void DrawBox(double x, double y, double width, double height)
{
	MovePen(x, y);
	DrawLine(width, 0);
	DrawLine(0, height);
	DrawLine(-width, 0);
	DrawLine(0, -height);
}

void Main()
{
	// Draw Title
	SetWindowTitle("Draw A House");
	InitGraphics();
	double width = GetWindowWidth();
	double height = GetWindowHeight();
	MovePen(TitleMargin, height - TitleMargin);
	SetPointSize(TitleSize);
	DrawTextString("Let's draw a house from scratch!");
	
	// Draw Roof
	double x = width / 2, y = height - HouseMargin;
	MovePen(x, y);
	DrawLine(-HouseWidth / 2, -RoofHeight);
	DrawLine(HouseWidth, 0);
	DrawLine(-HouseWidth / 2, RoofHeight);
	
	// Draw Chimney
	double slope = RoofHeight / (HouseWidth / 2);
	x = x - HouseWidth / 2 + ChimneyLeft;
	y = y - RoofHeight + slope * ChimneyLeft;
	MovePen(x, y);
	DrawLine(0, ChimneyHeight);
	DrawBox(x - ChimneyDelta, y + ChimneyHeight, BoxWidth, BoxHeight);
	x += ChimneyWidth;
	y += slope * ChimneyWidth;
	MovePen(x, y);
	DrawLine(0, ChimneyHeight - slope * ChimneyWidth);
	
	// Draw Body
	x = width / 2 - HouseWidth / 2;
	y = height - HouseMargin - RoofHeight - BodyHeight;
	DrawBox(x, y, HouseWidth, BodyHeight);
	x += HouseWidth / 2 + Spacing;
	DrawBox(x, y, DoorWidth, DoorHeight);
	x = x - 2 * Spacing - WindowWidth;
	y = y + BodyHeight - WindowTop - WindowHeight;
	DrawBox(x, y, WindowWidth, WindowHeight);
}
