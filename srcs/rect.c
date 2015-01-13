#include "rect.h"

#include <stdlib.h>

static inline double	_fabs(double value)
{
	if (value < 0.0)
		return (-value);
	return (value);
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
	double recta_left, recta_right, recta_top, recta_bottom;
	double rectb_left, rectb_right, rectb_top, rectb_bottom;

	recta_left = a_rect->origin.x - a_rect->halfsize.x;
	recta_right = a_rect->origin.x + a_rect->halfsize.x;
	recta_top = a_rect->origin.y + a_rect->halfsize.y;
	recta_bottom = a_rect->origin.y - a_rect->halfsize.y;
	rectb_left = b_rect->origin.x - b_rect->halfsize.x;
	rectb_right = b_rect->origin.x + b_rect->halfsize.x;
	rectb_top = b_rect->origin.y + b_rect->halfsize.y;
	rectb_bottom = b_rect->origin.y - b_rect->halfsize.y;
	if (recta_right < rectb_left || recta_left > rectb_right || recta_top < rectb_bottom || recta_bottom > rectb_top)
		return (false);
	else
		return (true);
}


