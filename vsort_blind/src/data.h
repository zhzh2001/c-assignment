#ifndef __DATA_H
#define __DATA_H

#define MaxValueInt 50
#define MaxValueFloat 5

extern int MinSize;
extern int MaxSize;
extern char Minbuf[5];
extern char Maxbuf[5];

/**
 * enum for colors, except merge sort colorful
 * C- prefix denotes 'color'
**/
typedef enum
{
	CNone,    /* not colored */
	CCurrent, /* current element highlighted */
	CSorted,  /* sorted region */
	CPivot,   /* pivot / reference element */
	CLeft,    /* left part (merge sort only) */
	CRight    /* right part (merge sort only) */
} color_t;

/**
 * enum for data types
 * T- prefix denotes 'type'
**/
typedef enum
{
	TInt, TFloat, TDouble
} data_t;

/**
 * core data structure
**/
typedef struct SortData
{
	int size;          /* size of data */
	data_t type;       /* data type */
	int *pInt;         /* int array pointer */
	int *pbkInt;       /* int array backup */
	float *pFloat;     /* float array pointer */
	float *pbkFloat;   /* float array backup */
	double *pDouble;   /* double array pointer */
	double *pbkDouble; /* double array backup */
	color_t *color;    /* colors of elements */
	// type = color_t except merge sort colorful ([int] leftmost index)
	double min;        /* min value */
	double max;        /* max value */
	// actually double (64-bit) could accurately store int, float, double
} SortData_t;

extern SortData_t data; // defined in data.c

/**
 * calculate bar length of data[index] for chart
 * return 0.5 if all values are equal (min = max)
 * @index index of data
 * @return value range [0.1, 1]
**/
double relativeRatio(int index);

/**
 * read from MergeData instead of data
**/
double relativeMergeRatio(int index);

/**
 * convert data to string, for chart label
 * @index index of data
 * @return string output
 * !free string after using!
**/
char *tostring(int index);

/**
 * read from MergeData instead of data
**/
char *toMergestring(int index);

/**
 * allocate data & color arrays
**/
void allocData();

/**
 * free arrays
**/
void freeData();

/**
 * backup / restore data
 * @backup backup to pbk    if TRUE
 *         restore from pbk if FALSE
**/
void transferData(bool backup);

/**
 * init min & max to data
**/
void initMinMax();

/**
 * randomly generate data
**/
void generateData();

/**
 * compare data[x] and data[y]
 * @x y indices of data
 * @return <0 if data[x] < data[y]
 *         =0 if data[x] = data[y]
 *         >0 if data[x] > data[y] 
**/
int cmp(int x, int y);

/**
 * swap data[x] and data[y]
**/
void swap(int x, int y);

/**
 * load data from file
**/
void loadData();

#endif
