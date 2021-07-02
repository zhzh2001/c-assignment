#ifndef __SELECT_H
#define __SELECT_H

/**
 * enum for selection sort event
**/
typedef enum
{
	InitMin, SelectIf, SetMin, SelectSwap, SelectSorted, PreSwap
} select_event;

/**
 * stores a selection sort event and params
**/
typedef struct SelNode
{
	select_event event; /* event type */
	int sorted;         /* number of sorted elements */
	int min;            /* index of current min value (pivot) */
	int now;            /* current index */
} SelNode_t;

/**
 * stores all data needed for selection sort playback
**/
typedef struct Sel_struct
{
	SelNode_t *selects;
	int min;
} Sel_t;

extern Sel_t SelectData;

/**
 * generate frames by selection sort
**/
void Selectsort();

/**
 * next frame by updating data and color
**/
void SelectNextStep();

/**
 * previous frame; exactly undo what NextStep did
**/
void SelectPrevStep();

#endif
