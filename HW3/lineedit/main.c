#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "extgraph.h"

#define TitleMargin 0.5
#define TitleSize 20
#define BufSize 256
#define BlinkInterval 500

char s90[] = "{|}";
char s58[] = ":+<_>?~\"";
char s58u[] = ";=,-./`'";
char s48[] = ")!@#$%^&*(";

double width, height;
char buf[BufSize];
int pos, len = 1; // '\0' included
bool erase, shift, cap;

void myTimerEvent(int timerID)
{
	char tmp = buf[pos];
	buf[pos] = '\0';
	MovePen(TitleMargin + TextStringWidth(buf), height - TitleMargin - GetFontDescent());
	SetEraseMode(erase);
	DrawLine(0, GetFontHeight());
	SetEraseMode(FALSE);
	erase = !erase;
	buf[pos] = tmp;
}

void myKeyboardEvent(int key, int event)
{
	if (event == KEY_DOWN)
	{
		if (key == 16) // shift
			shift = TRUE;
		if (key == 20) // caps lock
			cap = !cap;
		return;
	}
//	printf("%d\n", key);
	key &= 0x7f; // avoid extended ASCII, which would become Chinese characters
	switch (key)
	{
	case 13: // enter
		InitConsole();
		puts(buf);
		cancelKeyboardEvent();
		cancelTimerEvent();
		system("pause");
		ExitGraphics();
	case 37: // left
		if (pos > 0)
			pos--;
		break;
	case 39: // right
		if (pos < len - 1)
			pos++;
		break;
	case 8: // backspace
		if (pos > 0)
		{
			memmove(buf + pos - 1, buf + pos, len - pos + 1);
			pos--;
			len--;
		}
		break;
	case 46: // del
		if (pos < len - 1)
		{
			memmove(buf + pos, buf + pos + 1, len - pos);
			len--;
		}
		break;
	case 16: // shift
		shift = FALSE;
		break;
	case 17: // ctrl
		break;
	case 20: // caps lock
		break;
	case 127: // fn
		break;
	case 27: // esc
		ExitGraphics();
	case 38: // up
		break;
	case 40: // down
		break;
	case 35: // end
		pos = len - 1;
		break;
	case 36: // home
		pos = 0;
		break;
	default:
		if (key >= 58 && key <= 64)
			key = shift ? s58[key - 58] : s58u[key - 58];
		if (key == 94)
			key = shift ? '"' : '\'';
		if (shift)
		{
			if (key >= 91 && key <= 93)
				key = s90[key - 91];
			if (isdigit(key))
				key = s48[key - 48];
		}
		if (isalpha(key) && !shift && !cap)
			key = tolower(key);
		memmove(buf + pos + 1, buf + pos, len - pos);
		buf[pos++] = key;
		len++;
		break;
	}
	DisplayClear();
	MovePen(TitleMargin, height - TitleMargin);
	DrawTextString(buf);
	myTimerEvent(0);
}

void Main()
{
	// Draw Title
	SetWindowTitle("A Simple Line Editor");
	InitGraphics();
	width = GetWindowWidth();
	height = GetWindowHeight();
	MovePen(TitleMargin, height - TitleMargin);
	SetPointSize(TitleSize);
	SetFont("Consolas");
	DrawTextString("Let's edit with simple keyboard event processing!");
	registerKeyboardEvent(myKeyboardEvent);
	registerTimerEvent(myTimerEvent);
	startTimer(0, BlinkInterval);
//	InitConsole();
}
