#include "general.h"
#include "draw.h"
#include "data.h"
#include "bubble.h"
#include "select.h"
#include "merge.h"
#include "control.h"
#include "log.h"
#include "menu.h"

double winWidth, winHeight; // allocate

HWND hwndTrack, hwndProgress;

static int CodeLines[3] = {8, 7, 11};
static char Code[3][11][55] =
{
	{
		"{procedure} BubbleSort\0",
		"  {do}\0",
		"    swapped = {false}\0",
		"    {for} i = 1 {to} LastUnsortedElement - 1\0",
		"      {if} leftElement > rightElement\0",
		"        swap(leftElement, rightElement)\0",
		"        swapped = {true}\0",
		"  {while} swapped\0"
	},
	{
		"{procedure} SelectionSort\0",
		"  {repeat} (numOfElements - 1) times\0",
		"    set first unsorted element as minimum\0",
		"    {for} each of the unsorted elements\0",
		"      {if} element < minimum\0",
		"        set element as new minimum\0",
		"    swap minimum with unsorted position\0"
	},
	{
		"{procedure} MergeSort\0",
		"  {if} A contains one element: {return}\0",
		"  split A into two parts\0",
		"  MergeSort(left part of A)\0",
		"  MergeSort(right part of A)\0",
		"  {while} leftPart or rightPart is not empty\0",
		"    {if} leftPartHead < rightPartHead\0",
		"      copy leftPartHead to B\0",
		"    {else}\0",
		"      copy rightPartHead to B\0",
		"  copy B back to A\0"
	}
};

static int CodeHighlit[3][9] = 
{
	{2, 4, 0x65, 7},
	{2, 4, 5, 6, 1, 6},
	{1, 2, 3, 4, 7, 9, 10, 5, 10}
};

void drawChart()
{
	if (!data.color) // when data invalid, don't draw!
		return;
	double fontHeight = GetFontHeight();
	double menuHeight = fontHeight * 1.5;
	// menu bar / item height
	double BarWidth = (winWidth - ChartHoriMargin * 2) / data.size;
	// max width of a single bar
	double BarMargin = BarWidth / 10;
	// margin between bars
	double BarWidthA = BarWidth - BarMargin * 2;
	// actual width of a single bar with margin
	double BarHeight = winHeight - ChartVertMargin * 2 - menuHeight 
						- ToolbarHeight;
	// max height of bar
	double x = ChartHoriMargin + BarMargin;
	double y = ToolbarHeight + ChartVertMargin;
	MovePen(x, y);
	
	for (int i = 0; i < data.size; i++)
	{
		SetPenColor(palette[data.color[i]]);
		// set color with related color name
		double BarHeightA = BarHeight * relativeRatio(i);
		// actual height calculated by 'relativeRatio'
		StartFilledRegion(1.0);
		// filled rectangular / bar; density = 1.0 (full)
		DrawLine(BarWidthA, 0);
		DrawLine(0, BarHeightA);
		DrawLine(-BarWidthA, 0);
		DrawLine(0, -BarHeightA);
		EndFilledRegion();
		
		char *buf = tostring(i);
		if (TextStringWidth(buf) < BarWidthA) // avoid box overfull
		{
			double margin = (BarWidthA - TextStringWidth(buf)) / 2;
			// label left margin
			MovePen(x + margin, y + LabelMargin);
			SetPenColor("Black");
			DrawTextString(buf);
		}
		free(buf);
		
		x += BarWidth;
		MovePen(x, y);
	}
}

void drawMergeChart()
{
	if (!data.color)
		return;
	double fontHeight = GetFontHeight();
	double menuHeight = fontHeight * 1.5;
	double BarWidth = (winWidth - ChartHoriMargin * 2) / data.size;
	double BarMargin = BarWidth / 10;
	double BarWidthA = BarWidth - BarMargin * 2;
	double BarHeight = (winHeight - ToolbarHeight - ChartVertMargin * 3 
						- menuHeight) / 2;
	// half height
	double x = ChartHoriMargin + BarMargin;
	double y = ToolbarHeight + ChartVertMargin;
	MovePen(x, y);
	for (int i = 0; i < data.size; i++)
	{
		if (MergeData.copied[i]) // draw lower / copied part
		{
			SetPenColor(palette[data.color[i]]);
			double BarHeightA = BarHeight * relativeMergeRatio(i);
			// use 'merge' version!
			StartFilledRegion(1.0);
			DrawLine(BarWidthA, 0);
			DrawLine(0, BarHeightA);
			DrawLine(-BarWidthA, 0);
			DrawLine(0, -BarHeightA);
			EndFilledRegion();
			
			char *buf = toMergestring(i);
			// use 'merge' version!
			if (TextStringWidth(buf) < BarWidthA)
			{
				double margin = (BarWidthA - TextStringWidth(buf)) / 2;
				MovePen(x + margin, y + LabelMargin);
				SetPenColor("Black");
				DrawTextString(buf);
			}
			free(buf);
		}
		x += BarWidth;
		MovePen(x, y);
	}
	
	x = ChartHoriMargin + BarMargin;
	y = ToolbarHeight + ChartVertMargin * 2 + BarHeight;
	MovePen(x, y);
	for (int i = 0; i < data.size; i++)
	{
		if (MergeData.origin[i]) // draw upper / original part
		{
			SetPenColor(palette[data.color[i]]);
			double BarHeightA = BarHeight * relativeRatio(i);
			StartFilledRegion(1.0);
			DrawLine(BarWidthA, 0);
			DrawLine(0, BarHeightA);
			DrawLine(-BarWidthA, 0);
			DrawLine(0, -BarHeightA);
			EndFilledRegion();
			
			char *buf = tostring(i);
			if (TextStringWidth(buf) < BarWidthA)
			{
				double margin = (BarWidthA - TextStringWidth(buf)) / 2;
				MovePen(x + margin, y + LabelMargin);
				SetPenColor("Black");
				DrawTextString(buf);
			}
			free(buf);
		}
		x += BarWidth;
		MovePen(x, y);
	}
}

/* modify graphics.c to directly override with RGB color */
bool OverrideRGB;                    // if override
int OverrideR, OverrideG, OverrideB; // override RGB value, range [0, 255]

/**
 * [internal] convert from HSL to 'override' RGB
 * see https://www.rapidtables.com/convert/color/hsl-to-rgb.html
 * @h s l range [0, 1]
**/
static void hsl2rgb(double h, double s, double l)
{
	h *= 360; // convert to degree
	double c = (1 - fabs(2 * l - 1)) * s;
	double x = c * (1 - fabs(fmod(h / 60, 2) - 1));
	double m = l - c / 2;
	double r, g, b;
	switch ((int)(h / 60))
	{
	case 0:
		r = c; g = x; b = 0;
		break;
	case 1:
		r = x; g = c; b = 0;
		break;
	case 2:
		r = 0; g = c; b = x;
		break;
	case 3:
		r = 0; g = x; b = c;
		break;
	case 4:
		r = x; g = 0; b = c;
		break;
	case 5:
		r = c; g = 0; b = x;
		break;
	}
	OverrideR = (r + m) * 255;
	OverrideG = (g + m) * 255;
	OverrideB = (b + m) * 255;
}

void drawMergeChartColorful()
{
	if (!data.color)
		return;
	double fontHeight = GetFontHeight();
	double menuHeight = fontHeight * 1.5;
	double BarWidth = (winWidth - ChartHoriMargin * 2) / data.size;
	double BarMargin = BarWidth / 10;
	double BarWidthA = BarWidth - BarMargin * 2;
	double BarHeight = (winHeight - ToolbarHeight - ChartVertMargin * 3 
						- menuHeight) / 2;
	double x = ChartHoriMargin + BarMargin;
	double y = ToolbarHeight + ChartVertMargin;
	MovePen(x, y);
	for (int i = 0; i < data.size; i++)
	{
		if (MergeData.copied[i])
		{
			OverrideRGB = TRUE;
			// begin overriding
			if (MergeData.fade[i])
				hsl2rgb(0.8 * MergeData.color[i] / (data.size - 1), 0.5, 0.8);
				// fade by decrease saturation & increase lightness
				// max hue = 0.8 to avoid color overlapping 
			else
				hsl2rgb(0.8 * MergeData.color[i] / (data.size - 1), 1.0, 0.5);
			double BarHeightA = BarHeight * relativeMergeRatio(i);
			StartFilledRegion(1.0);
			DrawLine(BarWidthA, 0);
			DrawLine(0, BarHeightA);
			DrawLine(-BarWidthA, 0);
			DrawLine(0, -BarHeightA);
			EndFilledRegion();
			OverrideRGB = FALSE;
			// end overriding
			
			char *buf = toMergestring(i);
			if (TextStringWidth(buf) < BarWidthA)
			{
				double margin = (BarWidthA - TextStringWidth(buf)) / 2;
				MovePen(x + margin, y + LabelMargin);
				SetPenColor("Black");
				DrawTextString(buf);
			}
			free(buf);
		}
		x += BarWidth;
		MovePen(x, y);
	}
	
	x = ChartHoriMargin + BarMargin;
	y = ToolbarHeight + ChartVertMargin * 2 + BarHeight;
	MovePen(x, y);
	for (int i = 0; i < data.size; i++)
	{
		if (MergeData.origin[i])
		{
			OverrideRGB = TRUE;
			if (MergeData.fade[i])
				hsl2rgb(0.8 * data.color[i] / (data.size - 1), 0.5, 0.8);
			else
				hsl2rgb(0.8 * data.color[i] / (data.size - 1), 1.0, 0.5);
			double BarHeightA = BarHeight * relativeRatio(i);
			StartFilledRegion(1.0);
			DrawLine(BarWidthA, 0);
			DrawLine(0, BarHeightA);
			DrawLine(-BarWidthA, 0);
			DrawLine(0, -BarHeightA);
			EndFilledRegion();
			OverrideRGB = FALSE;
			
			char *buf = tostring(i);
			if (TextStringWidth(buf) < BarWidthA)
			{
				double margin = (BarWidthA - TextStringWidth(buf)) / 2;
				MovePen(x + margin, y + LabelMargin);
				SetPenColor("Black");
				DrawTextString(buf);
			}
			free(buf);
		}
		x += BarWidth;
		MovePen(x, y);
	}
}

void redraw()
{
	struct timespec start, stop;
	clock_gettime(CLOCK_MONOTONIC, &start);
	
	DisplayClear();
	if (ctrl.algo == AMerge)
		if (MergeData.colorful)
			drawMergeChartColorful();
		else
			drawMergeChart();
	else
		drawChart();
	drawToolbar();
	drawMenu();
	drawCode();
	
	clock_gettime(CLOCK_MONOTONIC, &stop);
	double time = (stop.tv_sec - start.tv_sec) + 
			(stop.tv_nsec - start.tv_nsec) * 1e-9;
	if (time > maxtime)
		maxtime = time;
	fprintf(flog, "redraw %.9lf s\n", time); // log redraw time
}

// CreateTrackbar - creates and initializes a trackbar. 
// 
// Global variable
//     g_hinst - instance handle
//
void WINAPI CreateBars( 
    HWND hwndDlg,  // handle of dialog box (parent window) 
    UINT iMin,     // minimum value in trackbar range 
    UINT iMax)     // maximum value in trackbar range 
{ 
	double x = ToolbarMargin + TextStringWidth("Speed: ");
	double y = ToolbarMargin;
    InitCommonControls(); // loads common control's DLL 

    hwndTrack = CreateWindowEx( 
        0,                               // no extended styles 
        TRACKBAR_CLASS,                  // class name 
        "Trackbar Control",              // title (caption) 
        WS_CHILD | 
        WS_VISIBLE | 
        TBS_AUTOTICKS,            // style 
        x * GetXResolution(),
		(winHeight - y) * GetYResolution() - 30,           // position 
        TrackbarWidth * GetXResolution(),
		30,                         // size 
        hwndDlg,                         // parent window 
        NULL,                     // control identifier 
        NULL,                         // instance 
        NULL                             // no WM_CREATE parameter 
        ); 

    SendMessage(hwndTrack, TBM_SETRANGE, 
        (WPARAM) TRUE,                   // redraw flag 
        (LPARAM) MAKELONG(iMin, iMax));  // min. & max. positions
        
    SendMessage(hwndTrack, TBM_SETPAGESIZE, 
        0, (LPARAM) 4);                  // new page size 
        
    SendMessage(hwndTrack, TBM_SETPOS, 
        (WPARAM) TRUE,                   // redraw flag 
        (LPARAM) DefInterval); 

//    SetFocus(hwndTrack); 
	SendMessage(hwndTrack, WM_CHANGEUISTATE,
	    MAKEWPARAM(UIS_SET, UISF_HIDEFOCUS), // hide focus style
		0);
	
	x = winWidth;
	hwndProgress = CreateWindowEx( 
        0,                               // no extended styles 
        PROGRESS_CLASS,                  // class name 
        "Progressbar Control",              // title (caption) 
        WS_CHILD | 
        WS_VISIBLE,            // style 
        x * GetXResolution(),
		(winHeight - y) * GetYResolution() - 30,           // position 
        ProgressbarWidth * GetXResolution(),
		30,                         // size 
        graphicsWindow,                         // parent window 
        NULL,                     // control identifier 
        NULL,                         // instance 
        NULL                             // no WM_CREATE parameter 
        ); 
}

void drawToolbar()
{
	double x = ToolbarMargin;
	double y = ToolbarMargin + GetFontHeight();
	MovePen(x, y);
	SetPenColor("Blue");
	DrawTextString("Speed: ");
	x += TrackbarWidth + 0.1 + TextStringWidth("Speed: ");
	MovePen(x, y);
	char buf[10];
	sprintf(buf, "%d ms", ctrl.interval);
	DrawTextString(buf);
	x += TextStringWidth("1000 ms") + ToolbarMargin;
	y = ToolbarMargin;
	if (button(GenUIID(0), x, y, ButtonWidth, ButtonHeight, "Play"))
		menuControlProcess(1);
	x += ButtonWidth + ButtonMargin;
	if (button(GenUIID(0), x, y, ButtonWidth, ButtonHeight, "Pause"))
		menuControlProcess(2);
	x += ButtonWidth + ButtonMargin;
	if (button(GenUIID(0), x, y, ButtonWidth, ButtonHeight, "Prev"))
		menuControlProcess(4);
	x += ButtonWidth + ButtonMargin;
	if (button(GenUIID(0), x, y, ButtonWidth, ButtonHeight, "Next"))
		menuControlProcess(3);
	x += ButtonWidth + ButtonMargin;
	if (button(GenUIID(0), x, y, ButtonWidth, ButtonHeight, "Reset"))
		menuControlProcess(5);

	x = winWidth;
	y = ToolbarHeight + ChartVertMargin + TextboxHeight * 2;
	MovePen(x, y + 0.08);
	SetPenColor("Blue");
	DrawTextString("Min Size: ");
	x += TextStringWidth("Max Size: ");
	textbox(GenUIID(0), x, y, TextboxWidth, TextboxHeight, Minbuf, 5);
	
	x = winWidth;
	y -= TextboxHeight * 2;
	MovePen(x, y + 0.08);
	SetPenColor("Blue");
	DrawTextString("Max Size: ");
	x += TextStringWidth("Max Size: ");
	textbox(GenUIID(0), x, y, TextboxWidth, TextboxHeight, Maxbuf, 5);
}

void drawCode()
{
	char *font = GetFont();
	int size = GetPointSize();
	SetFont("Consolas");
	SetPointSize(18);
	SetPenColor("Black");
	double fontHeight = GetFontHeight();
	double fontDesc = GetFontDescent();
	double menuHeight = fontHeight * 1.5;
	double x = winWidth;
	double y = winHeight - menuHeight - ChartVertMargin;
	MovePen(x, y);
	int highlight = -1;
	int algo = ctrl.algo;
	if (ctrl.curFrame && ctrl.interval >= 100)
		switch (algo)
		{
		case ABubble:
			highlight = CodeHighlit[0]
						[BubbleData.bubbles[ctrl.curFrame - 1].event];
			break;
		case ASelect:
			highlight = CodeHighlit[1]
						[SelectData.selects[ctrl.curFrame - 1].event];
			break;
		case AMerge:
			highlight = CodeHighlit[2]
						[MergeData.merges[ctrl.curFrame - 1].event];
			break;
		}
	for (int i = 0; i < CodeLines[algo]; i++)
	{
		SetPenColor("Black");
		if (highlight == i || (highlight & 15) == i || 
			(highlight >= 16 && highlight / 16 == i))
		{
			StartFilledRegion(1.0);
			double width = 15.0 - x - ChartVertMargin;
			y -= fontDesc;
			MovePen(x, y);
			DrawLine(width, 0);
			DrawLine(0, fontHeight * 1.5);
			DrawLine(-width, 0);
			DrawLine(0, -fontHeight * 1.5);
			EndFilledRegion();
			y += fontDesc;
			MovePen(x, y);
			SetPenColor("White");
		}
		char *p = strchr(Code[algo][i], '{');
		char *prev = Code[algo][i];
		while (p)
		{
			*p = '\0';
			DrawTextString(prev);
			*p++ = '{';
			char *q = strchr(p, '}');
			*q = '\0';
			SetStyle(Bold);
			DrawTextString(p);
			SetStyle(Normal);
			*q = '}';
			p = strchr(q, '{');
			prev = q + 1;
		}
		DrawTextString(prev);
		x = winWidth;
		y -= fontHeight * 1.5;
		MovePen(x, y);
	}
	SetFont(font);
	SetPointSize(size);
	SetPenColor("Blue");
}

/**
 * [internal] resize window by raw Windows API
 * not used as is buggy
**/
void ResizeWindow(double wfactor, double hfactor)
{
	HWND desktop;
	extern HWND graphicsWindow;
	RECT bounds;
	extern int pixelWidth, pixelHeight;
	
	desktop = GetDesktopWindow();
	GetWindowRect(desktop, &bounds);
	pixelWidth = (bounds.right - bounds.left) / wfactor;
	pixelHeight = (bounds.bottom - bounds.top) / hfactor;
	SetWindowPos(graphicsWindow, 0, 0, 0, pixelWidth, pixelHeight, 0);
	
	winWidth = pixelWidth / GetXResolution();
	winHeight = pixelHeight / GetYResolution();
	fprintf(flog, "%lf %lf\n", winWidth, winHeight);
}
