#include "general.h"
#include "select.h"
#include "data.h"
#include "control.h"

Sel_t SelectData;

void Selectsort()
{
	transferData(FALSE);
	int frames = 0;
	for (int i = 0; i < data.size - 1; i++)
	{
		int min = i;
		frames++;
		for (int j = i + 1; j < data.size; j++)
		{
			frames++;
			if (cmp(j, min) < 0)
			{
				min = j;
				frames++;
			}
		}
		if (min != i)
			frames++;
		frames++;
		swap(min, i);
		frames++;
	}
	
	ctrl.totalFrame = frames;
	SelectData.selects = (SelNode_t *)malloc(sizeof(SelNode_t) * frames);
	
	transferData(FALSE);
	frames = 0;
	for (int i = 0; i < data.size - 1; i++)
	{
		int min = i;
		SelectData.selects[frames].event = InitMin;
		SelectData.selects[frames++].min = i;
		for (int j = i + 1; j < data.size; j++)
		{
			SelectData.selects[frames].event = SelectIf;
			SelectData.selects[frames++].now = j;
			if (cmp(j, min) < 0)
			{
				SelectData.selects[frames].now = min;
				min = j;
				SelectData.selects[frames].event = SetMin;
				SelectData.selects[frames++].min = j;
			}
		}
		if (min != i)
		{
			SelectData.selects[frames].event = PreSwap;
			SelectData.selects[frames].min = min;
			SelectData.selects[frames++].now = i;
		}
		swap(min, i);
		SelectData.selects[frames].event = SelectSwap;
		SelectData.selects[frames].min = min;
		SelectData.selects[frames++].now = i;
		
		SelectData.selects[frames].event = SelectSorted;
		SelectData.selects[frames++].sorted = i;
	}
}

void SelectNextStep()
{
	SelNode_t now = SelectData.selects[ctrl.curFrame];
	SelNode_t prev = ctrl.curFrame ? SelectData.selects[ctrl.curFrame - 1]
						: now;
	
	switch (now.event)
	{
	case InitMin:
		data.color[now.min] = CPivot;
		break;
	case SelectIf:
		if (prev.event == SelectIf)
			data.color[prev.now] = CNone;
		data.color[now.now] = CCurrent;
		break;
	case SetMin:
		data.color[now.now] = CNone;
		data.color[now.min] = CPivot;
		break;
	case PreSwap:
		if (prev.event == SelectIf)
			data.color[prev.now] = CNone;
		data.color[now.now] = CPivot;
		break;
	case SelectSwap:
		if (prev.event != PreSwap)
		{
			if (prev.event == SelectIf)
				data.color[prev.now] = CNone;
			data.color[now.now] = CPivot;
		}
		swap(now.min, now.now);
		break;
	case SelectSorted:
		data.color[prev.min] = CNone;
		data.color[prev.now] = CNone;
		data.color[now.sorted] = CSorted;
		break;
	}
}

void SelectPrevStep()
{
	SelNode_t now = SelectData.selects[ctrl.curFrame];
	SelNode_t prev = ctrl.curFrame ? SelectData.selects[ctrl.curFrame - 1]
						: now;
	
	switch (now.event)
	{
	case InitMin:
		data.color[now.min] = CNone;
		break;
	case SelectIf:
		data.color[now.now] = CNone;
		if (prev.event == SelectIf)
			data.color[prev.now] = CCurrent;
		break;
	case SetMin:
		data.color[now.min] = CNone;
		data.color[now.now] = CPivot;
		break;
	case PreSwap:
		if (now.now != now.min)
			data.color[now.now] = CNone;
		if (prev.event == SelectIf)
			data.color[prev.now] = CCurrent;
		break;
	case SelectSwap:
		swap(now.min, now.now);
		if (prev.event != PreSwap)
		{
			if (now.now != now.min)
			data.color[now.now] = CNone;
			if (prev.event == SelectIf)
				data.color[prev.now] = CCurrent;
		}
		break;
	case SelectSorted:
		data.color[now.sorted] = CNone;
		data.color[prev.min] = CPivot;
		data.color[prev.now] = CPivot;
		break;
	}
}
