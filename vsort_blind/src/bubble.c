#include "general.h"
#include "bubble.h"
#include "data.h"
#include "control.h"

Bubble_t BubbleData;

void Bubblesort()
{
	transferData(FALSE);
	int last = data.size;
	int frames = 0;
	bool swapped;
	do
	{
		swapped = FALSE;
		frames++;
		for (int i = 0; i < last - 1; i++)
		{
			frames++;
			if (cmp(i, i + 1) > 0)
			{
				swap(i, i + 1);
				swapped = TRUE;
				frames++;
			}
		}
		last--; // last unsorted element
		frames++;
	} while (swapped); // optimize: break if no swapping
	// first run: determine total frames
	
	BubbleData.bubbles = (BubbleNode_t *)
							malloc(sizeof(BubbleNode_t) * frames);
	ctrl.totalFrame = frames;
	
	transferData(FALSE);
	last = data.size;
	frames = 0;
	do
	{
		swapped = FALSE;
		BubbleData.bubbles[frames++].event = ClearSwap;
		for (int i = 0; i < last - 1; i++)
		{
			BubbleData.bubbles[frames].event = BubbleIf;
			BubbleData.bubbles[frames++].current = i;
			if (cmp(i, i + 1) > 0)
			{
				swap(i, i + 1);
				swapped = TRUE;
				BubbleData.bubbles[frames].event = BubbleSwap;
				BubbleData.bubbles[frames++].current = i;
			}
		}
		last--;
		BubbleData.bubbles[frames].event = BubbleLast;
		BubbleData.bubbles[frames++].last = last;
	} while (swapped);
}

void BubbleNextStep()
{
	BubbleNode_t now = BubbleData.bubbles[ctrl.curFrame];
	BubbleNode_t prev = ctrl.curFrame ? BubbleData.bubbles[ctrl.curFrame - 1]
						: now; // prevent out of bound
	
	switch (now.event)
	{
	case ClearSwap:
		break;
	case BubbleIf:
		if (prev.event == BubbleIf || prev.event == BubbleSwap)
		// don't clear color if previous is BubbleLast
		{
			data.color[prev.current] = CNone;
			data.color[prev.current + 1] = CNone;
		}
		data.color[now.current] = CCurrent;
		data.color[now.current + 1] = CCurrent;
		break;
	case BubbleSwap:
		swap(now.current, now.current + 1);
		break;
	case BubbleLast:
		BubbleData.last = now.last;
		data.color[prev.current] = CNone;
		data.color[prev.current + 1] = CNone;
		data.color[now.last] = CSorted;
		break;
	}
}

void BubblePrevStep()
{
	BubbleNode_t now = BubbleData.bubbles[ctrl.curFrame];
	BubbleNode_t prev = ctrl.curFrame ? BubbleData.bubbles[ctrl.curFrame - 1]
						: now;
	
	switch (now.event)
	{
	case ClearSwap:
		break;
	case BubbleIf:
		data.color[now.current] = CNone;
		data.color[now.current + 1] = CNone;
		if (prev.event == BubbleIf || prev.event == BubbleSwap)
		{
			data.color[prev.current] = CCurrent;
			data.color[prev.current + 1] = CCurrent;
		}
		break;
	case BubbleSwap:
		swap(now.current, now.current + 1);
		break;
	case BubbleLast:
		data.color[now.last] = CNone;
		data.color[prev.current] = CCurrent;
		data.color[prev.current + 1] = CCurrent;
		BubbleData.last++;
		break;
	}
}
