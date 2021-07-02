#ifndef __MENU_H
#define __MENU_H

#define MenuMargin 0.2

/* Menu lists start */
static char * menuListFile[] = 
{
	"File",
	"Load State... | Ctrl-O", // ... denotes dialog box required
	"Save State... | Ctrl-S",
	"Quit"
};

static char *menuListControl[] = 
{
	"Control",
	"Play",
	"Pause",
	"Next Step | Ctrl-N", // hotkey
	"Prev Step | Ctrl-B",
	"Reset       | Ctrl-R"
};

static char *menuListData[] = 
{
	"Data",
	"Random Int",
	"Random Float",
	"Random Double",
	"From File..."
};

static char *menuListAlgo[] =
{
	"Algorithm",
	"Bubble Sort",
	"Selection Sort",
	"Merge Sort",
	"Merge Sort Colorful"
};

static char *menuListHelp[] =
{
	"Help",
	"About..."
};

/**
 * Process file menu, concerning load / save state
 * @selection start from 1
**/ 
void menuFileProcess(int selection);

/**
 * Control menu, concerning play navigation
**/
void menuControlProcess(int selection);

/**
 * Data menu, concerning randomize / load data
**/
void menuDataProcess(int selection);

/**
 * Algorithm menu, concerning switch sorting algorithms
**/
void menuAlgoProcess(int selection);

/**
 * Help menu, concerning about
**/
void menuHelpProcess(int selection);

/**
 * Render the menu 
**/
void drawMenu();

#endif
