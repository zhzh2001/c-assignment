#include "general.h"
#include "control.h"
#include "data.h"
#include "bubble.h"
#include "select.h"
#include "merge.h"
#include "draw.h"

Control_t ctrl; // allocate core control structure

int intervals[IntervalCount] = {3000, 2000, 1600, 1300, 1000, 800, 600, 500,
								400, 300, 200, 100, 50, 20, 5, 0};

void init()
{
	if (ctrl.status == SRaw)
	{
		sort();
		ctrl.status = SSorted; // would not leak to outside
	}
	if (ctrl.status == SSorted)
	{
		transferData(FALSE);   // restore
		ctrl.curFrame = 0;     // first frame
		SendMessage(hwndProgress, PBM_SETPOS, 0, 0);
		ctrl.status = SSorting;
	}
}

void sort()
{
	switch (ctrl.algo)
	{
	case ABubble:
		free(BubbleData.bubbles);
		Bubblesort();
		BubbleData.last = data.size; // last element
		break;
	case ASelect:
		free(SelectData.selects);
		Selectsort();
		break;
	case AMerge:
		free(MergeData.merges);
		Mergesort();
		break;
	}
	SendMessage(hwndProgress, PBM_SETRANGE32, 0, ctrl.totalFrame);
	SendMessage(hwndProgress, PBM_SETSTEP, 1, 0);
}

void NextStep()
{
	switch (ctrl.algo)
	{
	case ABubble:
		BubbleNextStep();
		break;
	case ASelect:
		SelectNextStep();
		break;
	case AMerge:
		if (MergeData.colorful) // determine whether colorful
			MergeNextStepColorful();
		else
			MergeNextStep();
		break;
	}
	
	ctrl.curFrame++;
	SendMessage(hwndProgress, PBM_STEPIT, 0, 0);
	if (ctrl.curFrame == ctrl.totalFrame) // animation completed
	{
		cancelTimer(0);
		ctrl.playing = FALSE;
		clearFrame();
		ctrl.curFrame = 0;
		SendMessage(hwndProgress, PBM_SETPOS, 0, 0);
		ctrl.status = SSorted;
	}
}

void PrevStep()
{
	if (!ctrl.curFrame) // first frame, no previous
		return;
	ctrl.curFrame--;    // undo last frame
	SendMessage(hwndProgress, PBM_DELTAPOS, -1, 0);
	switch (ctrl.algo)
	{
	case ABubble:
		BubblePrevStep();
		break;
	case ASelect:
		SelectPrevStep();
		break;
	case AMerge:
		if (MergeData.colorful)
			MergePrevStepColorful();
		else
			MergePrevStep();
		break;
	}
}

void clearFrame()
{
	memset(data.color, CNone, sizeof(color_t) * data.size);
	if (MergeData.fade)
		for (int i = 0; i < data.size; i++)
		{
			MergeData.fade[i] = FALSE;   // not fade
			MergeData.copied[i] = FALSE;
			MergeData.origin[i] = TRUE;  // bars all on the upper part
		}
}

void loadState()
{
	cancelTimer(0);
	ctrl.playing = FALSE;
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260] = {0};
	
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = 0;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "State Files (*.sav)\0*.sav\0";
	// only *.sav supported
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = ".sav"; // default file extension
	ofn.lpstrTitle = "Load State From";
	
	if (GetOpenFileName(&ofn))
	{
		freeData();
		FILE *fp = fopen(szFile, "rb"); // binary file
		fread(&data.size, sizeof(int), 1, fp);
		fread(&data.type, sizeof(data_t), 1, fp);
		allocData();
		switch (data.type)
		{
		case TInt:
			fread(data.pInt, sizeof(int), data.size, fp);
			fread(data.pbkInt, sizeof(int), data.size, fp);
			break;
		case TFloat:
			fread(data.pFloat, sizeof(float), data.size, fp);
			fread(data.pbkFloat, sizeof(float), data.size, fp);
			break;
		case TDouble:
			fread(data.pDouble, sizeof(double), data.size, fp);
			fread(data.pbkDouble, sizeof(double), data.size, fp);
			break;
		}
		fread(data.color, sizeof(color_t), data.size, fp);
		fread(&data.min, sizeof(double), 1, fp);
		fread(&data.max, sizeof(double), 1, fp);
		
		fread(&ctrl, sizeof(ctrl), 1, fp);
		for (int i = 0; i < IntervalCount; i++)
			if (ctrl.interval == intervals[i])
			{
				SendMessage(hwndTrack, TBM_SETPOS, TRUE, i);
				break;
			}
		SendMessage(hwndProgress, PBM_SETRANGE32, 0, ctrl.totalFrame);
		SendMessage(hwndProgress, PBM_SETSTEP, 1, 0);
		
		switch (ctrl.algo)
		{
		case ABubble:
			fread(&BubbleData.last, sizeof(int), 1, fp);
			free(BubbleData.bubbles);
			BubbleData.bubbles = (BubbleNode_t *)
							malloc(sizeof(BubbleNode_t) * ctrl.totalFrame);
			fread(BubbleData.bubbles, sizeof(BubbleNode_t), ctrl.totalFrame,
					fp);
			break;
			
		case ASelect:
			fread(&SelectData.min, sizeof(int), 1, fp);
			free(SelectData.selects);
			SelectData.selects = (SelNode_t *)
							malloc(sizeof(SelNode_t) * ctrl.totalFrame);
			fread(SelectData.selects, sizeof(SelNode_t), ctrl.totalFrame, fp);
			break;
			
		case AMerge:
			fread(&MergeData.colorful, sizeof(bool), 1, fp);
			switch (data.type)
			{
			case TInt:
				free(MergeData.pInt);
				MergeData.pInt = (int *)malloc(sizeof(int) * data.size);
				fread(MergeData.pInt, sizeof(int), data.size, fp);
				break;
			case TFloat:
				free(MergeData.pFloat);
				MergeData.pFloat = (float *)malloc(sizeof(float) * data.size);
				fread(MergeData.pFloat, sizeof(float), data.size, fp);
				break;
			case TDouble:
				free(MergeData.pDouble);
				MergeData.pDouble = (double *)malloc(sizeof(double) * data.size);
				fread(MergeData.pDouble, sizeof(double), data.size, fp);
				break;
			}
			
			free(MergeData.merges);
			MergeData.merges = (MergeNode_t *)
							malloc(sizeof(MergeNode_t) * ctrl.totalFrame);
			fread(MergeData.merges, sizeof(MergeNode_t), ctrl.totalFrame, fp);
			
			free(MergeData.copied);
			MergeData.copied = (bool *)malloc(sizeof(bool) * data.size);
			fread(MergeData.copied, sizeof(bool), data.size, fp);
			
			free(MergeData.origin);
			MergeData.origin = (bool *)malloc(sizeof(bool) * data.size);
			fread(MergeData.origin, sizeof(bool), data.size, fp);
			
			if (MergeData.colorful)
			{
				free(MergeData.fade);
				MergeData.fade = (bool *)malloc(sizeof(bool) * data.size);
				fread(MergeData.fade, sizeof(bool), data.size, fp);
				
				free(MergeData.color);
				MergeData.color = (int *)malloc(sizeof(int) * data.size);
				fread(MergeData.color, sizeof(int), data.size, fp);
			}
			break;
		}
		
		fclose(fp);
		redraw();
	}
}

void saveState()
{
	cancelTimer(0);
	ctrl.playing = FALSE;
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260] = {0};
	
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = 0;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "State Files (*.sav)\0*.sav\0";
	ofn.lpstrDefExt = "sav";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST;
	ofn.lpstrDefExt = ".sav";
	ofn.lpstrTitle = "Save State As";
	
	if (GetOpenFileName(&ofn))
	{
		FILE *fp = fopen(szFile, "wb");
		fwrite(&data.size, sizeof(int), 1, fp);
		fwrite(&data.type, sizeof(data_t), 1, fp);
		switch (data.type)
		{
		case TInt:
			fwrite(data.pInt, sizeof(int), data.size, fp);
			fwrite(data.pbkInt, sizeof(int), data.size, fp);
			break;
		case TFloat:
			fwrite(data.pFloat, sizeof(float), data.size, fp);
			fwrite(data.pbkFloat, sizeof(float), data.size, fp);
			break;
		case TDouble:
			fwrite(data.pDouble, sizeof(double), data.size, fp);
			fwrite(data.pbkDouble, sizeof(double), data.size, fp);
			break;
		}
		fwrite(data.color, sizeof(color_t), data.size, fp);
		fwrite(&data.min, sizeof(double), 1, fp);
		fwrite(&data.max, sizeof(double), 1, fp);
		
		fwrite(&ctrl, sizeof(ctrl), 1, fp);
		
		switch (ctrl.algo)
		{
		case ABubble:
			fwrite(&BubbleData.last, sizeof(int), 1, fp);
			fwrite(BubbleData.bubbles, sizeof(BubbleNode_t), ctrl.totalFrame,
					fp);
			break;
			
		case ASelect:
			fwrite(&SelectData.min, sizeof(int), 1, fp);
			fwrite(SelectData.selects, sizeof(SelNode_t), ctrl.totalFrame, fp);
			break;
			
		case AMerge:
			fwrite(&MergeData.colorful, sizeof(bool), 1, fp);
			switch (data.type)
			{
			case TInt:
				fwrite(MergeData.pInt, sizeof(int), data.size, fp);
				break;
			case TFloat:
				fwrite(MergeData.pFloat, sizeof(float), data.size, fp);
				break;
			case TDouble:
				fwrite(MergeData.pDouble, sizeof(double), data.size, fp);
				break;
			}
			
			fwrite(MergeData.merges, sizeof(MergeNode_t), ctrl.totalFrame, fp);
			fwrite(MergeData.copied, sizeof(bool), data.size, fp);
			fwrite(MergeData.origin, sizeof(bool), data.size, fp);
			
			if (MergeData.colorful)
			{
				fwrite(MergeData.fade, sizeof(bool), data.size, fp);
				fwrite(MergeData.color, sizeof(int), data.size, fp);
			}
			break;
		}
		fclose(fp);
	}
}
