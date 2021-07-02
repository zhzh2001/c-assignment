#include "angular.h"
#include <math.h>
#include "extgraph.h"

static double angular; // in degree
double pi = acos(-1);

void forward(double distance)
{
	double dxi = cos(angular / 180 * pi);
	double dyi = sin(angular / 180 * pi);
	DrawLine(distance * dxi, distance * dyi);
}

void turn(double angle)
{
	angular += angle;
}

void move(double distance)
{
	double dxi = cos(angular / 180 * pi);
	double dyi = sin(angular / 180 * pi);
	double x = GetCurrentX(), y = GetCurrentY();
	MovePen(x + distance * dxi, y + distance * dyi);
}
