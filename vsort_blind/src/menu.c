#include "general.h"
#include "menu.h"
#include "log.h"
#include "control.h"
#include "data.h"
#include "merge.h"
#include "draw.h"

void menuFileProcess(int selection)
{
	switch (selection)
	{
	case 1: /* load state from file */
		loadState();
		break;
		
	case 2: /* save state to file */
		saveState();
		break;
		
	case 3: /* quit */
		freeData();
		fprintf(flog, "%.9lf s\n", maxtime);
		ExitGraphics();
		break;
	}
}

void menuControlProcess(int selection)
{
	switch (selection)
	{
	case 1: /* start playing */
		init();
		startTimer(0, ctrl.interval);
		ctrl.playing = TRUE;
		break;
		
	case 2: /* pause playing */
		cancelTimer(0);
		ctrl.playing = FALSE;
		break;
		
	case 3: /* next step */
		init();
		cancelTimer(0); // if playing, pause it
		ctrl.playing = FALSE;
		NextStep();
		redraw();
		break;
		
	case 4: /* previous step */
		cancelTimer(0);
		ctrl.playing = FALSE;
		PrevStep();
		redraw();
		break;
		
	case 5: /* reset */
		cancelTimer(0);
		ctrl.playing = FALSE;
		transferData(FALSE); // restore data
		clearFrame();
		ctrl.curFrame = 0;
		SendMessage(hwndProgress, PBM_SETPOS, 0, 0);
		redraw();
		break;
	}
}

void menuDataProcess(int selection)
{
	switch (selection)
	{
	case 1: /* random int */
		data.type = TInt;
		generateData();
		break;
		
	case 2: /* random float */
		data.type = TFloat;
		generateData();
		break;
		
	case 3: /* random double */
		data.type = TDouble;
		generateData();
		break;
		
	case 4: /* load data from file */
		loadData();
		break;
	}
	
	if (selection)
	{
		if (ctrl.algo == AMerge)
			init(); // required for correctly drawing
		redraw();
	}
}

void menuAlgoProcess(int selection)
{
	if (selection) // reset on changing algorithm
	{
		cancelTimer(0);
		ctrl.playing = FALSE;
		ctrl.status = SRaw; // frames not created
		transferData(FALSE);
		clearFrame();
		ctrl.curFrame = 0;
		SendMessage(hwndProgress, PBM_SETPOS, 0, 0);
	}
	
	switch (selection)
	{
	case 1: /* bubble sort */
		ctrl.algo = ABubble;
		break;
		
	case 2: /* selection sort */
		ctrl.algo = ASelect;
		break;
		
	case 3: /* merge sort with simple color */
		ctrl.algo = AMerge;
		MergeData.colorful = FALSE;
		init(); // also required for proper drawing
		break;
		
	case 4: /* merge sort colorful */
		ctrl.algo = AMerge;
		MergeData.colorful = TRUE;
		init();
		break;
	}
}

void menuHelpProcess(int selection)
{
	switch (selection)
	{
	case 1:
		MessageBox(NULL, "Sort Visualization by <name not allowed in blind review>",
		          "About", 0);
		break;
	}
}

void drawMenu()
{
	double fontHeight = GetFontHeight();
	double menuHeight = fontHeight * 1.5;
	// menu bar / item height
	
	double menuBarWidth1 = TextStringWidth(menuListFile[0]) + MenuMargin;
	double menuBarWidth2 = TextStringWidth(menuListControl[0]) + MenuMargin;
	double menuBarWidth3 = TextStringWidth(menuListData[0]) + MenuMargin;
	double menuBarWidth4 = TextStringWidth(menuListAlgo[0]) + MenuMargin;
	double menuBarWidth5 = TextStringWidth(menuListHelp[0]) + MenuMargin;
	double menuBarWidth = menuBarWidth1 + menuBarWidth2 + menuBarWidth3 +
	                      menuBarWidth4 + menuBarWidth5;
	// width of only 'file' menu 
	double menuListWidth = TextStringWidth(menuListFile[1]) + MenuMargin;
	// widest 'file' menu item                         ^-- widest item
	drawMenuBar(0, winHeight - menuHeight, menuBarWidth, menuHeight);
	int sel = menuList(GenUIID(0), 0 /* no bar on left */,
						winHeight - menuHeight /* bottom y */, menuBarWidth1,
						menuListWidth, menuHeight, menuListFile,
						sizeof(menuListFile) / sizeof(menuListFile[0]));
	menuFileProcess(sel);
	
	menuListWidth = TextStringWidth(menuListControl[3]) + MenuMargin;
	sel = menuList(GenUIID(0), menuBarWidth1 /* 'file' menu bar on left */,
					winHeight - menuHeight, menuBarWidth2,
					menuListWidth, menuHeight, menuListControl,
					sizeof(menuListControl) / sizeof(menuListControl[0]));
	menuControlProcess(sel);
	
	menuListWidth = TextStringWidth(menuListData[3]) + MenuMargin;
	sel = menuList(GenUIID(0), menuBarWidth1 + menuBarWidth2 /* 2 menu bars */,
					winHeight - menuHeight, menuBarWidth3,
					menuListWidth, menuHeight, menuListData,
					sizeof(menuListData) / sizeof(menuListData[0]));
	menuDataProcess(sel);
	
	menuListWidth = TextStringWidth(menuListAlgo[4]) + MenuMargin;
	sel = menuList(GenUIID(0), menuBarWidth1 + menuBarWidth2 + menuBarWidth3,
					winHeight - menuHeight, menuBarWidth4,
					menuListWidth, menuHeight, menuListAlgo,
					sizeof(menuListAlgo) / sizeof(menuListAlgo[0]));
	menuAlgoProcess(sel);
	
	menuListWidth = TextStringWidth(menuListHelp[1]) + MenuMargin;
	sel = menuList(GenUIID(0), menuBarWidth1 + menuBarWidth2 + menuBarWidth3 +
					menuBarWidth4, winHeight - menuHeight, menuBarWidth5,
					menuListWidth, menuHeight, menuListHelp,
					sizeof(menuListHelp) / sizeof(menuListHelp[0]));
	menuHelpProcess(sel);
}
