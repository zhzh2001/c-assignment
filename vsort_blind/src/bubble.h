#ifndef __BUBBLE_H
#define __BUBBLE_H

/**
 * enum for bubble sort event
**/
typedef enum
{
	ClearSwap, BubbleIf, BubbleSwap, BubbleLast
} bubble_event;

/**
 * stores a bubble sort event and params
**/ 
typedef struct BubbleNode
{
	bubble_event event; /* event type */
	int current;        /* i when comparing a[i] and a[i+1] */
	int last;           /* last unsorted element; used when updating */
} BubbleNode_t;

/**
 * stores all data needed for bubble sort playback
**/
typedef struct Bubble_struct
{
	BubbleNode_t *bubbles;
	int last; // useless
} Bubble_t;

extern Bubble_t BubbleData;

/**
 * generate frames by bubble sort
**/
void Bubblesort();

/**
 * next frame by updating data and color
**/
void BubbleNextStep();

/**
 * previous frame; exactly undo what NextStep did
**/
void BubblePrevStep();

#endif
