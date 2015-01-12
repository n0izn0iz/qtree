#include "rect.h"

#include <stdlib.h>

static inline int		_abs(int value)
{
	if (value < 0)
		return (-value);
	return (value);
}

static inline double	_fabs(double value)
{
	if (value < 0.0)
		return (-value);
	return (value);
}


t_irect					irect_create(t_ipoint origin, t_ipoint halfsize)
{
	t_irect		rect;

	rect.origin = origin;
	rect.halfsize = halfsize;
	return (rect);
}

t_irect*				irect_alloc(t_ipoint origin, t_ipoint halfsize)
{
	t_irect*	rect;

	rect = (t_irect*)malloc(sizeof(t_irect));
	if (rect != NULL)
	{
		rect->origin = origin;
		rect->halfsize = halfsize;
	}
	return (rect);
}

bool					irect_containsipoint(const t_irect* rect, const t_ipoint* point)
{
	if (_abs(point->x - rect->origin.x) <= rect->halfsize.x && _abs(point->y - rect->origin.y) <= rect->halfsize.y)
		return (true);
	return (false);
}

bool					irect_intersect(const t_irect* a_rect, const t_irect* b_rect)
{
	t_ipoint point;

	point.x = b_rect->origin.x - b_rect->halfsize.x;
	point.y = b_rect->origin.y - b_rect->halfsize.y;
	if (irect_containsipoint(a_rect, &point))
		return (true);
	point.y = b_rect->origin.y + b_rect->halfsize.y;
	if (irect_containsipoint(a_rect, &point))
		return (true);
	point.x = b_rect->origin.x + b_rect->halfsize.x;
	if (irect_containsipoint(a_rect, &point))
		return (true);
	point.x = b_rect->origin.x - b_rect->halfsize.x;
	point.y = b_rect->origin.y + b_rect->halfsize.y;
	if (irect_containsipoint(a_rect, &point))
		return (true);
	return (false);
}

t_frect					frect_create(t_fpoint origin, t_fpoint halfsize)
{
	t_frect		rect;

	rect.origin = origin;
	rect.halfsize = halfsize;
	return (rect);
}

t_frect*				frect_alloc(t_fpoint origin, t_fpoint halfsize)
{
	t_frect*	rect;

	rect = (t_frect*)malloc(sizeof(t_frect));
	if (rect != NULL)
	{
		rect->origin = origin;
		rect->halfsize = halfsize;
	}
	return (rect);
}

bool					frect_containsfpoint(const t_frect* rect, const t_fpoint* point)
{
	if (point->x >= rect->origin.x - rect->halfsize.x && point->x <= rect->origin.x + rect->halfsize.x
		&& point->y >= rect->origin.y - rect->halfsize.y && point->y <= rect->origin.y + rect->halfsize.y)
		return (true);
	return (false);
}

bool					frect_intersect(const t_frect* a_rect, const t_frect* b_rect)
{
	t_fpoint point;

	point.x = b_rect->origin.x - b_rect->halfsize.x;
	point.y = b_rect->origin.y - b_rect->halfsize.y;
	if (frect_containsfpoint(a_rect, &point))
		return (true);
	point.y = b_rect->origin.y + b_rect->halfsize.y;
	if (frect_containsfpoint(a_rect, &point))
		return (true);
	point.x = b_rect->origin.x + b_rect->halfsize.x;
	if (frect_containsfpoint(a_rect, &point))
		return (true);
	point.x = b_rect->origin.x - b_rect->halfsize.x;
	point.y = b_rect->origin.y + b_rect->halfsize.y;
	if (frect_containsfpoint(a_rect, &point))
		return (true);
	point.x = a_rect->origin.x - a_rect->halfsize.x;
	point.y = a_rect->origin.y - a_rect->halfsize.y;
	if (frect_containsfpoint(b_rect, &point))
		return (true);
	point.y = a_rect->origin.y + a_rect->halfsize.y;
	if (frect_containsfpoint(b_rect, &point))
		return (true);
	point.x = a_rect->origin.x + a_rect->halfsize.x;
	if (frect_containsfpoint(b_rect, &point))
		return (true);
	point.x = a_rect->origin.x - a_rect->halfsize.x;
	point.y = a_rect->origin.y + a_rect->halfsize.y;
	if (frect_containsfpoint(b_rect, &point))
		return (true);
	return (false);
}


