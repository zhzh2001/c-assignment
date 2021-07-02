#include "general.h"
#include "merge.h"
#include "data.h"
#include "control.h"

Merge_t MergeData;

/*
Mergesort(A)
1. if A contains one element: return
2. split A into two parts
3. Mergesort(left part of A)
4. Mergesort(right part of A)
5. while leftPart or rightPart is not empty
6.   if leftPartHeadValue < rightPartHeadValue
7.     copy leftPartHead to B
8.   else
9.     copy rightPartHead to B
10. copy B back to A
*/

void PreMergesort(int l, int r)
{
	if (l + 1 == r) // single element
	{
		ctrl.totalFrame++;
		return;
	}
	int mid = (l + r) / 2;
	ctrl.totalFrame++;
	PreMergesort(l, mid);
	ctrl.totalFrame++;
	PreMergesort(mid, r);
	ctrl.totalFrame++;
	ctrl.totalFrame++;
	
	// here start two-way merge
	int now = l;
	for (int i = l, j = mid; i < mid || j < r; )
	{
		if (j == r || (i < mid && cmp(i, j) < 0))
			switch (data.type)
			{
			case TInt:
				MergeData.pInt[now++] = data.pInt[i++];
				break;
			case TFloat:
				MergeData.pFloat[now++] = data.pFloat[i++];
				break;
			case TDouble:
				MergeData.pDouble[now++] = data.pDouble[i++];
				break;
			}
		else
			switch (data.type)
			{
			case TInt:
				MergeData.pInt[now++] = data.pInt[j++];
				break;
			case TFloat:
				MergeData.pFloat[now++] = data.pFloat[j++];
				break;
			case TDouble:
				MergeData.pDouble[now++] = data.pDouble[j++];
				break;
			}
		ctrl.totalFrame++;
	}
	
	// copy back
	for (int i = l; i < r; i++)
		switch (data.type)
		{
		case TInt:
			data.pInt[i] = MergeData.pInt[i];
			break;
		case TFloat:
			data.pFloat[i] = MergeData.pFloat[i];
			break;
		case TDouble:
			data.pDouble[i] = MergeData.pDouble[i];
			break;
		}
	ctrl.totalFrame++;
	ctrl.totalFrame++;
}

void InMergesort(int l, int r)
{
	if (l + 1 == r)
	{
		MergeData.merges[ctrl.totalFrame].event = Return;
		MergeData.merges[ctrl.totalFrame++].l = l;
		return;
	}
	MergeData.merges[ctrl.totalFrame].event = Split;
	MergeData.merges[ctrl.totalFrame].l = l;
	MergeData.merges[ctrl.totalFrame++].r = r;
	int mid = (l + r) / 2;
	MergeData.merges[ctrl.totalFrame].event = MergeLeft;
	MergeData.merges[ctrl.totalFrame].l = l;
	MergeData.merges[ctrl.totalFrame++].r = r;
	InMergesort(l, mid);
	MergeData.merges[ctrl.totalFrame].event = MergeRight;
	MergeData.merges[ctrl.totalFrame].l = l;
	MergeData.merges[ctrl.totalFrame++].r = r;
	InMergesort(mid, r);
	MergeData.merges[ctrl.totalFrame].event = Fill;
	MergeData.merges[ctrl.totalFrame].l = l;
	MergeData.merges[ctrl.totalFrame++].r = r;
	
	// here start two-way merge
	int now = l;
	for (int i = l, j = mid; i < mid || j < r; )
	{
		if (j == r || (i < mid && cmp(i, j) < 0))
		{
			MergeData.merges[ctrl.totalFrame].event = CopyLeft;
			MergeData.merges[ctrl.totalFrame].l = now;
			MergeData.merges[ctrl.totalFrame++].r = i;
			switch (data.type)
			{
			case TInt:
				MergeData.pInt[now++] = data.pInt[i++];
				break;
			case TFloat:
				MergeData.pFloat[now++] = data.pFloat[i++];
				break;
			case TDouble:
				MergeData.pDouble[now++] = data.pDouble[i++];
				break;
			}
		}
		else
		{
			MergeData.merges[ctrl.totalFrame].event = CopyRight;
			MergeData.merges[ctrl.totalFrame].l = now;
			MergeData.merges[ctrl.totalFrame++].r = j;
			switch (data.type)
			{
			case TInt:
				MergeData.pInt[now++] = data.pInt[j++];
				break;
			case TFloat:
				MergeData.pFloat[now++] = data.pFloat[j++];
				break;
			case TDouble:
				MergeData.pDouble[now++] = data.pDouble[j++];
				break;
			}
		}
	}
	
	// copy back
	for (int i = l; i < r; i++)
		switch (data.type)
		{
		case TInt:
			data.pInt[i] = MergeData.pInt[i];
			break;
		case TFloat:
			data.pFloat[i] = MergeData.pFloat[i];
			break;
		case TDouble:
			data.pDouble[i] = MergeData.pDouble[i];
			break;
		}
	MergeData.merges[ctrl.totalFrame].event = CopyBack;
	MergeData.merges[ctrl.totalFrame].l = l;
	MergeData.merges[ctrl.totalFrame++].r = r;
	MergeData.merges[ctrl.totalFrame].event = Unfill;
	MergeData.merges[ctrl.totalFrame].l = l;
	MergeData.merges[ctrl.totalFrame++].r = r;
}

void Mergesort()
{
	transferData(FALSE);
	free(MergeData.copied);
	MergeData.copied = (bool *)malloc(sizeof(bool) * data.size);
	free(MergeData.origin);
	MergeData.origin = (bool *)malloc(sizeof(bool) * data.size);
	free(MergeData.fade);
	MergeData.fade = (bool *)malloc(sizeof(bool) * data.size);
	free(MergeData.color);
	MergeData.color = (int *)malloc(sizeof(int) * data.size);
	for (int i = 0; i < data.size; i++)
	{
		MergeData.copied[i] = FALSE;
		MergeData.origin[i] = TRUE;
		MergeData.fade[i] = FALSE;
	}
	
	switch (data.type)
	{
	case TInt:
		free(MergeData.pInt);
		MergeData.pInt = (int *)malloc(sizeof(int) * data.size);
		break;
	case TFloat:
		free(MergeData.pFloat);
		MergeData.pFloat = (float *)malloc(sizeof(float) * data.size);
		break;
	case TDouble:
		free(MergeData.pDouble);
		MergeData.pDouble = (double *)malloc(sizeof(double) * data.size);
		break;
	}
	ctrl.totalFrame = 0;
	PreMergesort(0, data.size);
	
	transferData(FALSE);
	MergeData.merges = (MergeNode_t *)malloc(sizeof(MergeNode_t) *
						ctrl.totalFrame);
	ctrl.totalFrame = 0;
	InMergesort(0, data.size);
}

void MergeNextStep()
{
	MergeNode_t now = MergeData.merges[ctrl.curFrame];
	int mid = (now.l + now.r) / 2;
	switch (now.event)
	{
	case Return:
		data.color[now.l] = CNone;
		break;
	case Split:
		for (int i = now.l; i < mid; i++)
			data.color[i] = CLeft;
		for (int i = mid; i < now.r; i++)
			data.color[i] = CRight;
		break;
	case MergeLeft:
		for (int i = now.l; i < mid; i++)
			data.color[i] = CPivot;
		for (int i = mid; i < now.r; i++)
			data.color[i] = CNone;
		break;
	case MergeRight:
		for (int i = now.l; i < mid; i++)
			data.color[i] = CNone;
		for (int i = mid; i < now.r; i++)
			data.color[i] = CPivot;
		break;
	case CopyLeft:
		MergeData.copied[now.l] = TRUE;
		MergeData.origin[now.r] = FALSE;
		switch (data.type)
		{
		case TInt:
			MergeData.pInt[now.l] = data.pInt[now.r];
			break;
		case TFloat:
			MergeData.pFloat[now.l] = data.pFloat[now.r];
			break;
		case TDouble:
			MergeData.pDouble[now.l] = data.pDouble[now.r];
			break;
		}
		break;
	case CopyRight:
		MergeData.copied[now.l] = TRUE;
		MergeData.origin[now.r] = FALSE;
		switch (data.type)
		{
		case TInt:
			MergeData.pInt[now.l] = data.pInt[now.r];
			break;
		case TFloat:
			MergeData.pFloat[now.l] = data.pFloat[now.r];
			break;
		case TDouble:
			MergeData.pDouble[now.l] = data.pDouble[now.r];
			break;
		}
		break;
	case CopyBack:
		for (int i = now.l; i < now.r; i++)
		{
			switch (data.type)
			{
			case TInt:
				data.pInt[i] = MergeData.pInt[i];
				break;
			case TFloat:
				data.pFloat[i] = MergeData.pFloat[i];
				break;
			case TDouble:
				data.pDouble[i] = MergeData.pDouble[i];
				break;
			}
			MergeData.copied[i] = FALSE;
			MergeData.origin[i] = TRUE;
		}
		break;
	case Fill:
	// start two-way merge; fill color
		for (int i = now.l; i < now.r; i++)
			data.color[i] = CCurrent;
		break;
	case Unfill:
		for (int i = now.l; i < now.r; i++)
			data.color[i] = CNone;
		break;
	}
}

void MergeNextStepColorful()
{
	MergeNode_t now = MergeData.merges[ctrl.curFrame];
	int mid = (now.l + now.r) / 2;
	switch (now.event)
	{
	case Return:
		data.color[now.l] = now.l;
		break;
	case Split:
		for (int i = now.l; i < mid; i++)
			data.color[i] = now.l;
		for (int i = mid; i < now.r; i++)
			data.color[i] = mid;
		break;
	case MergeLeft:
		for (int i = mid; i < now.r; i++)
			MergeData.fade[i] = TRUE;
		// fade right half
		break;
	case MergeRight:
		for (int i = now.l; i < mid; i++)
			MergeData.fade[i] = TRUE;
		// fade left half
		for (int i = mid; i < now.r; i++)
			MergeData.fade[i] = FALSE;
		// restore right half
		break;
	case CopyLeft:
		MergeData.copied[now.l] = TRUE;
		MergeData.origin[now.r] = FALSE;
		switch (data.type)
		{
		case TInt:
			MergeData.pInt[now.l] = data.pInt[now.r];
			break;
		case TFloat:
			MergeData.pFloat[now.l] = data.pFloat[now.r];
			break;
		case TDouble:
			MergeData.pDouble[now.l] = data.pDouble[now.r];
			break;
		}
		break;
	case CopyRight:
		MergeData.copied[now.l] = TRUE;
		MergeData.origin[now.r] = FALSE;
		switch (data.type)
		{
		case TInt:
			MergeData.pInt[now.l] = data.pInt[now.r];
			break;
		case TFloat:
			MergeData.pFloat[now.l] = data.pFloat[now.r];
			break;
		case TDouble:
			MergeData.pDouble[now.l] = data.pDouble[now.r];
			break;
		}
		break;
	case CopyBack:
		for (int i = now.l; i < now.r; i++)
		{
			switch (data.type)
			{
			case TInt:
				data.pInt[i] = MergeData.pInt[i];
				break;
			case TFloat:
				data.pFloat[i] = MergeData.pFloat[i];
				break;
			case TDouble:
				data.pDouble[i] = MergeData.pDouble[i];
				break;
			}
			MergeData.copied[i] = FALSE;
			MergeData.origin[i] = TRUE;
			data.color[i] = now.l; // sorted = uniform color
		}
		break;
	case Fill:
		for (int i = now.l; i < now.r; i++)
		{
			MergeData.fade[i] = FALSE;
			MergeData.color[i] = now.l; // merged = uniform color
		}
		break;
	case Unfill:
		for (int i = now.l; i < now.r; i++)
			MergeData.fade[i] = TRUE;
		break;
	}
}

void MergePrevStep()
{
	MergeNode_t now = MergeData.merges[ctrl.curFrame];
	int mid = (now.l + now.r) / 2;
	switch (now.event)
	{
	case Return:
		data.color[now.l] = CPivot;
		break;
	case Split:
		for (int i = now.l; i < now.r; i++)
			if (now.l == 0 && now.r == data.size)
				data.color[i] = CNone;
			else
				data.color[i] = CPivot;
		break;
	case MergeLeft:
		for (int i = now.l; i < mid; i++)
			data.color[i] = CLeft;
		for (int i = mid; i < now.r; i++)
			data.color[i] = CRight;
		break;
	case MergeRight:
		for (int i = mid; i < now.r; i++)
			data.color[i] = CNone;
		break;
	case CopyLeft:
		MergeData.copied[now.l] = FALSE;
		MergeData.origin[now.r] = TRUE;
		switch (data.type)
		{
		case TInt:
			data.pInt[now.r] = MergeData.pInt[now.l];
			break;
		case TFloat:
			data.pFloat[now.r] = MergeData.pFloat[now.l];
			break;
		case TDouble:
			data.pDouble[now.r] = MergeData.pDouble[now.l];
			break;
		}
		break;
	case CopyRight:
		MergeData.copied[now.l] = FALSE;
		MergeData.origin[now.r] = TRUE;
		switch (data.type)
		{
		case TInt:
			data.pInt[now.r] = MergeData.pInt[now.l];
			break;
		case TFloat:
			data.pFloat[now.r] = MergeData.pFloat[now.l];
			break;
		case TDouble:
			data.pDouble[now.r] = MergeData.pDouble[now.l];
			break;
		}
		break;
	case CopyBack:
		for (int i = now.l; i < now.r; i++)
		{
			switch (data.type)
			{
			case TInt:
				MergeData.pInt[i] = data.pInt[i];
				break;
			case TFloat:
				MergeData.pFloat[i] = data.pFloat[i];
				break;
			case TDouble:
				MergeData.pDouble[i] = data.pDouble[i];
				break;
			}
			MergeData.copied[i] = TRUE;
			MergeData.origin[i] = FALSE;
		}
		break;
	case Fill:
		for (int i = now.l; i < now.r; i++)
			data.color[i] = CNone;
		break;
	case Unfill:
		for (int i = now.l; i < now.r; i++)
			data.color[i] = CCurrent;
		break;
	}
}

void MergePrevStepColorful()
{
	MergeNode_t now = MergeData.merges[ctrl.curFrame];
	int mid = (now.l + now.r) / 2;
	switch (now.event)
	{
	case Return:
		break;
	case Split:
		for (int i = now.l; i < now.r; i++)
			data.color[i] = now.l;
		break;
	case MergeLeft:
		for (int i = now.l; i < mid; i++)
			data.color[i] = now.l;
		for (int i = mid; i < now.r; i++)
		{
			data.color[i] = mid;
			MergeData.fade[i] = FALSE;
		}
		break;
	case MergeRight:
		for (int i = now.l; i < mid; i++)
			MergeData.fade[i] = FALSE;
		for (int i = mid; i < now.r; i++)
			MergeData.fade[i] = TRUE;
		break;
	case CopyLeft:
		MergeData.copied[now.l] = FALSE;
		MergeData.origin[now.r] = TRUE;
		switch (data.type)
		{
		case TInt:
			data.pInt[now.r] = MergeData.pInt[now.l];
			break;
		case TFloat:
			data.pFloat[now.r] = MergeData.pFloat[now.l];
			break;
		case TDouble:
			data.pDouble[now.r] = MergeData.pDouble[now.l];
			break;
		}
		break;
	case CopyRight:
		MergeData.copied[now.l] = FALSE;
		MergeData.origin[now.r] = TRUE;
		switch (data.type)
		{
		case TInt:
			data.pInt[now.r] = MergeData.pInt[now.l];
			break;
		case TFloat:
			data.pFloat[now.r] = MergeData.pFloat[now.l];
			break;
		case TDouble:
			data.pDouble[now.r] = MergeData.pDouble[now.l];
			break;
		}
		break;
	case CopyBack:
		for (int i = now.l; i < now.r; i++)
		{
			switch (data.type)
			{
			case TInt:
				MergeData.pInt[i] = data.pInt[i];
				break;
			case TFloat:
				MergeData.pFloat[i] = data.pFloat[i];
				break;
			case TDouble:
				MergeData.pDouble[i] = data.pDouble[i];
				break;
			}
			MergeData.copied[i] = TRUE;
			MergeData.origin[i] = FALSE;
		}
		for (int i = now.l; i < mid; i++)
			data.color[i] = now.l;
		for (int i = mid; i < now.r; i++)
			data.color[i] = mid;
		break;
	case Fill:
		for (int i = now.l; i < mid; i++)
		{
			MergeData.color[i] = now.l;
			MergeData.fade[i] = TRUE;
		}
		for (int i = mid; i < now.r; i++)
		{
			MergeData.color[i] = mid;
			MergeData.fade[i] = TRUE;
		}
		break;
	case Unfill:
		for (int i = now.l; i < now.r; i++)
			MergeData.fade[i] = FALSE;
		break;
	}
}
