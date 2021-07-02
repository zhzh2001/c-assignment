#ifndef __MERGE_H
#define __MERGE_H

/**
 * enum for merge sort event
**/
typedef enum
{
	Return, Split, MergeLeft, MergeRight, CopyLeft, CopyRight, CopyBack,
	Fill, Unfill
} merge_event;

/**
 * stores a merge sort event and params
**/
typedef struct MergeNode
{
	merge_event event; /* event type */
	int l, r;          /* range [l, r) or assignment a[l] = a[r] */
} MergeNode_t;

/**
* stores all data needed for merge sort playback
**/
typedef struct Merge_struct
{
	bool colorful;
	MergeNode_t *merges;
	bool *copied, *origin; /* if the element is copied down or remained up */
	bool *fade;            /* fade when not currently sorting */
	int *color;            /* color of merged (copied down) element */
	// color[i] denotes the leftmost index of the sequence it belongs to
	int *pInt;             /* merged data */
	float *pFloat;
	double *pDouble;
} Merge_t;

extern Merge_t MergeData;

/**
 * merge sort first run with recursion
**/
void PreMergesort(int l, int r);

/**
 * merge sort second run, creating frames
**/
void InMergesort(int l, int r);

/**
 * generate frames by selection sort
 * call PreMergesort & InMergesort
**/
void Mergesort();

/**
 * next frame by updating data and color (non-colorful)
**/
void MergeNextStep();

/**
 * next frame; except for colorful version
**/ 
void MergeNextStepColorful();

/**
 * previous frame; exactly undo what NextStep did (non-colorful)
**/
void MergePrevStep();

/**
 * previous frame; except for colorful version
**/
void MergePrevStepColorful();

#endif
