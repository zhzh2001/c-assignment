#ifndef __DRAW_H
#define __DRAW_H

#define ChartVertMargin 0.5
#define ChartHoriMargin 0.5
#define LabelMargin 0.1 // label vertical margin
#define ToolbarHeight 0.8
#define ToolbarMargin 0.5
#define ButtonHeight (ToolbarHeight - ToolbarMargin)
#define ButtonWidth 0.8
#define ButtonMargin 0.2
#define TrackbarWidth 2.0
#define ProgressbarWidth 4.5
#define TextboxWidth 2.0
#define TextboxHeight 0.3

/**
 * [internal] color palette
**/
static char *palette[] = 
{
	"Cyan",   /* CNone */
	"Green",  /* CCurrent */
	"Blue",   /* CSorted */
	"Red",    /* CPivot */
	"Orange", /* CLeft */
	"Brown"   /* CRight */
};

extern double winWidth, winHeight;
// window width & height

extern HWND hwndTrack, hwndProgress;

/**
 * draw chart, except merge sort
**/
void drawChart();

/**
 * draw chart, merge sort with simple color only
**/
void drawMergeChart();

/**
 * draw chart, merge sort colorful only
**/
void drawMergeChartColorful();

/**
 * redraw menu & chart
**/
void redraw();

/**
 * draw toolbar on the bottom
**/
void drawToolbar();

/**
 * draw code on the right
**/
void drawCode();

void WINAPI CreateBars( 
    HWND hwndDlg,  // handle of dialog box (parent window) 
    UINT iMin,     // minimum value in trackbar range 
    UINT iMax);    // maximum value in trackbar range

#endif
