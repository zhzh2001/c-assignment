#ifndef __CONTROL_H
#define __CONTROL_H

/**
 * enum for algorithm type
 * A- prefix denotes 'algorithm'
**/
typedef enum
{
	ABubble, ASelect, AMerge
} algo_t;

/**
 * enum for status
 * S- prefix denotes 'status'
**/
typedef enum
{
	SRaw,     /* raw data, no frames created */
	SSorting, /* currently sorting */
	SSorted   /* already sorted */
} status_t;

/**
 * core control structure
**/
typedef struct Control_struct
{
	int curFrame;    /* current frame index, range [0, total) */
	int totalFrame;  /* total frames */
	algo_t algo;     /* algorithm selected */
	status_t status; /* status */
	int interval;    /* interval */
	bool playing;
} Control_t;

extern Control_t ctrl; // defined in control.c

#define IntervalCount 16
extern int intervals[IntervalCount];
#define DefInterval 7

/**
 * init frames if not & reset if sorted
**/
void init();

/**
 * create frames with corresponding algorithm
**/
void sort();

/**
 * display next step with corresponding algorithm
 * decode from frame information created by 'sort'
**/
void NextStep();

/**
 * display previous step with corresponding algorithm
 * apparently harder than 'NextStep'
**/
void PrevStep();

/**
 * set all color to CNone / not fade
**/
void clearFrame();

/**
 * load state from binary file *.sav
**/
void loadState();

/**
 * save state to binary file *.sav
**/
void saveState();

#endif
