#include "general.h"
#include "data.h"
#include "draw.h"
#include "control.h"
#include "log.h"

/**
 * Char event callback for imgui
 * Redraw after GUI update
**/
static void CharEventProcess(char ch)
{
	uiGetChar(ch);
	redraw();
}

/**
 * Keyboard event callback for imgui
**/
static void KeyboardEventProcess(int key, int event)
{
	uiGetKeyboard(key, event);
	redraw();
}

/**
 * Mouse event callback for imgui
**/
static void MouseEventProcess(int x, int y, int button, int event)
{
	uiGetMouse(x, y, button, event);
	redraw();
}

/**
 * Timer event callback for playing animation
 * Next step on timer
**/
static void TimerEventProcess(int timerID)
{
	NextStep();
	redraw();
}

/**
 * Init GUI for libgraphics
**/
void Main(HINSTANCE hInstance)
{
	flog = fopen("vsort.log", "w"); // init log file
	srand(time(NULL));              // init random function
	SetWindowTitle("vsort - Sorting Algorithm Visualization");
	SetWindowSize(15.0, 7.0);
	setMenuColors("White", "Black", "Blue", "White", 1);
	InitGraphics();
	
	winWidth = 10.0;
	winHeight = GetWindowHeight();
	fprintf(flog, "%lf %lf\n", winWidth, winHeight);
	
	HICON hIcon = LoadIcon(hInstance, "A");
	SendMessage(graphicsWindow, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	SendMessage(graphicsWindow, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	
	generateData(); // generate random int data, can play out of box
	
	registerCharEvent(CharEventProcess);
	registerKeyboardEvent(KeyboardEventProcess);
	registerMouseEvent(MouseEventProcess);
	registerTimerEvent(TimerEventProcess);
	redraw(); // first draw
	CreateBars(graphicsWindow, 0, IntervalCount - 1);
	ctrl.interval = 500;
}
