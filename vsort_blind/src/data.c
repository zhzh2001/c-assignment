#include "general.h"
#include "data.h"
#include "merge.h"
#include "control.h"

int MinSize = 10;
int MaxSize = 20;
char Minbuf[5] = "10";
char Maxbuf[5] = "20";

SortData_t data; // allocate core data structure

double relativeRatio(int index)
{
	double range = data.max - data.min;
	if (!range)
		return 0.5; // avoid division by 0
	double ans;
	switch (data.type)
	{
	case TInt:
		ans = (data.pInt[index] - data.min) / range;
		break;
	case TFloat:
		ans = (data.pFloat[index] - data.min) / range;
		break;
	case TDouble:
		ans = (data.pDouble[index] - data.min) / range;
		break;
	}
	return 0.9 * ans + 0.1; // constraint to [0.1, 1]
}

double relativeMergeRatio(int index)
{
	double range = data.max - data.min;
	if (!range)
		return 0.5;
	double ans;
	switch (data.type)
	{
	case TInt:
		ans = (MergeData.pInt[index] - data.min) / range;
		break;
	case TFloat:
		ans = (MergeData.pFloat[index] - data.min) / range;
		break;
	case TDouble:
		ans = (MergeData.pDouble[index] - data.min) / range;
		break;
	}
	return 0.9 * ans + 0.1;
}

char *tostring(int index)
{
	char *buf = malloc(10); // 10 bytes seem enough
	switch (data.type)
	{
	case TInt:
		sprintf(buf, "%d", data.pInt[index]);
		break;
	case TFloat:
		sprintf(buf, "%.2f", data.pFloat[index]);
		break;
	case TDouble:
		sprintf(buf, "%.2lf", data.pDouble[index]);
		break;
	}
	return buf;
}

char *toMergestring(int index)
{
	char *buf = malloc(10);
	switch (data.type)
	{
	case TInt:
		sprintf(buf, "%d", MergeData.pInt[index]);
		break;
	case TFloat:
		sprintf(buf, "%.2f", MergeData.pFloat[index]);
		break;
	case TDouble:
		sprintf(buf, "%.2lf", MergeData.pDouble[index]);
		break;
	}
	return buf;
}

/**
 * [internal] randomly generate int in [l, r]
**/
static int random(int l, int r)
{
	int n = rand();
	if (RAND_MAX == 32767 && r - l >= RAND_MAX)
		n = (n << 15) + rand(); // to support wider range
	return n % (r - l + 1) + l;
}

/**
 * [internal] randomly generate double in [0, max]
**/
static double randomFloat(double max)
{
	return 1.0 * rand() / RAND_MAX * max;
}

void allocData()
{
	switch (data.type)
	{
	case TInt:
		data.pInt = (int *)malloc(sizeof(int) * data.size);
		data.pbkInt = (int *)malloc(sizeof(int) * data.size);
		// also allocate backup array
		break;
		
	case TFloat:
		data.pFloat = (float *)malloc(sizeof(float) * data.size);
		data.pbkFloat = (float *)malloc(sizeof(float) * data.size);
		break;
		
	case TDouble:
		data.pDouble = (double *)malloc(sizeof(double) * data.size);
		data.pbkDouble = (double *)malloc(sizeof(double) * data.size);
		break;
	}
	data.color = (color_t *)malloc(sizeof(color_t) * data.size);
	memset(data.color, CNone, sizeof(color_t) * data.size);
}

void freeData()
{
	switch (data.type)
	{
	case TInt:
		free(data.pInt);
		free(data.pbkInt);
		data.pInt = NULL;
		data.pbkInt = NULL; // avoid wild pointer
		break;
		
	case TFloat:
		free(data.pFloat);
		free(data.pbkFloat);
		data.pFloat = NULL;
		data.pbkFloat = NULL;
		break;
		
	case TDouble:
		free(data.pDouble);
		free(data.pbkDouble);
		data.pDouble = NULL;
		data.pbkDouble = NULL;
		break;
	}
	free(data.color);
	data.color = NULL;
}

void transferData(bool backup)
{
	switch (data.type)
	{
	case TInt:
		if (backup)
			memcpy(data.pbkInt, data.pInt, sizeof(int) * data.size);
		else
			memcpy(data.pInt, data.pbkInt, sizeof(int) * data.size);
		break;
		
	case TFloat:
		if (backup)
			memcpy(data.pbkFloat, data.pFloat, sizeof(float) * data.size);
		else
			memcpy(data.pFloat, data.pbkFloat, sizeof(float) * data.size);
		break;
		
	case TDouble:
		if (backup)
			memcpy(data.pbkDouble, data.pDouble, sizeof(double) * data.size);
		else
			memcpy(data.pDouble, data.pbkDouble, sizeof(double) * data.size);
		break;
	}
}

void initMinMax()
{
	switch (data.type)
	{
	case TInt:
		data.min = data.pInt[0];
		data.max = data.pInt[0];
		break;
	case TFloat:
		data.min = data.pFloat[0];
		data.max = data.pFloat[0];
		break;
	case TDouble:
		data.min = data.pDouble[0];
		data.max = data.pDouble[0];
		break;
	}
	
	for (int i = 1; i < data.size; i++)
		switch (data.type)
		{
		case TInt:
			if (data.pInt[i] < data.min)
				data.min = data.pInt[i];
			if (data.pInt[i] > data.max)
				data.max = data.pInt[i];
			break;
			
		case TFloat:
			if (data.pFloat[i] < data.min)
				data.min = data.pFloat[i];
			if (data.pFloat[i] > data.max)
				data.max = data.pFloat[i];
			break;
			
		case TDouble:
			if (data.pDouble[i] < data.min)
				data.min = data.pDouble[i];
			if (data.pDouble[i] > data.max)
				data.max = data.pDouble[i];
			break;
		}
}

void generateData()
{
	cancelTimer(0); // pause to avoid trouble 
	freeData();
	MinSize = atoi(Minbuf);
	MaxSize = atoi(Maxbuf);
	if (MaxSize < MinSize)
	{
		MaxSize = MinSize;
		strcpy(Maxbuf, Minbuf);
	}
	data.size = random(MinSize, MaxSize);
	allocData(); // reallocate
	
	for (int i = 0; i < data.size; i++)
		switch (data.type)
		{
		case TInt:
			data.pInt[i] = random(0, MaxValueInt);
			break;
		case TFloat:
			data.pFloat[i] = randomFloat(MaxValueFloat);
			break;
		case TDouble:
			data.pDouble[i] = randomFloat(MaxValueFloat);
			break;
		}
	initMinMax();
	transferData(TRUE); // backup newly generated data
	ctrl.status = SRaw;
}

int cmp(int x, int y)
{
	switch (data.type)
	{
	case TInt:
		return data.pInt[x] - data.pInt[y];
	case TFloat:
		if (data.pFloat[x] < data.pFloat[y]) // return value is int...
			return -1;
		if (data.pFloat[x] > data.pFloat[y])
			return 1;
		return 0;
		
	case TDouble:
		if (data.pDouble[x] < data.pDouble[y])
			return -1;
		if (data.pDouble[x] > data.pDouble[y])
			return 1;
		return 0;
	}
}

void swap(int x, int y)
{
	int tmp;
	double ftmp;
	switch (data.type)
	{
	case TInt:
		tmp = data.pInt[x];
		data.pInt[x] = data.pInt[y];
		data.pInt[y] = tmp;
		break;
		
	case TFloat:
		ftmp = data.pFloat[x];
		data.pFloat[x] = data.pFloat[y];
		data.pFloat[y] = ftmp;
		break;
		
	case TDouble:
		ftmp = data.pDouble[x];
		data.pDouble[x] = data.pDouble[y];
		data.pDouble[y] = ftmp;
		break;
	}
}

/*
snippet from
https://stackoverflow.com/questions/4167286/win32-function-to-openfiledialog 
*/
void loadData()
{
	cancelTimer(0);
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260] = {0};
	
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = 0;      // no owner, seemingly without trouble
	ofn.lpstrFile = szFile; // filename storage
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	// accept *.txt and *.*
	ofn.nFilterIndex = 1;   // default filter index (*.txt)
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	// path and file must exist
	ofn.lpstrDefExt = ".txt";          // default extension if unspecified
	ofn.lpstrTitle = "Load Data From"; // dialog title
	
	if (GetOpenFileName(&ofn))         // call common dialog!
	{
		freeData();
		FILE *fp = fopen(szFile, "r");
		char type[10];
		fscanf(fp, "%s", type);
		if (!strcmp(type, "int"))
			data.type = TInt;
		else if (!strcmp(type, "float"))
			data.type = TFloat;
		else if (!strcmp(type, "double"))
			data.type = TDouble;
		else
		{
			MessageBox(0, "Invalid file!\nNow generating random data",
						"Error", MB_ICONERROR);
			generateData();
			return;
		}
		
		data.size = 0;
		while (!feof(fp))
		{
			if (fscanf(fp, "%s", type) == 1)
				data.size++;
			else
				break;
		}
		// count size of data 
		
		rewind(fp);             // reset
		allocData();
		fscanf(fp, "%s", type); // pass #2, type is useless
		int now = 0;
		while (!feof(fp))
		{
			switch (data.type)
			{
			case TInt:
				fscanf(fp, "%d", &data.pInt[now++]);
				break;
			case TFloat:
				fscanf(fp, "%f", &data.pFloat[now++]);
				break;
			case TDouble:
				fscanf(fp, "%lf", &data.pDouble[now++]);
				break;
			}
			if (now == data.size) // don't overread
				break;
		}
		fclose(fp);
		initMinMax();
		transferData(TRUE);       // backup
		ctrl.status = SRaw;
	}
}
