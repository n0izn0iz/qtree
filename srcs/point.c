#include "point.h"

#include <stdlib.h>

t_ipoint		ipoint_create(int x, int y)
{
	t_ipoint	point;

	point.x = x;
	point.y = y;
	return (point);
}

t_ipoint*		ipoint_alloc(int x, int y)
{
	t_ipoint*	point;

	point = malloc(sizeof(t_ipoint));
	if (point != NULL)
	{
		point->x = x;
		point->y = y;
	}
	return (point);
}

t_fpoint		fpoint_create(double x, double y)
{
	t_fpoint	point;

	point.x = x;
	point.y = y;
	return (point);
}

t_fpoint*		fpoint_alloc(double x, double y)
{
	t_fpoint*	point;

	point = malloc(sizeof(t_fpoint));
	if (point != NULL)
	{
		point->x = x;
		point->y = y;
	}
	return (point);
}
