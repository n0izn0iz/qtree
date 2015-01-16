#include "srect.h"

#include <stdlib.h>
#include "vecmath.h"
#include "shapetype.h"

t_shape*	srect_create(double hfx, double hfy, uint32_t color, double x, double y)
{
	t_srect*		srect;

	srect = (t_srect*)malloc(sizeof(t_srect));
	if (srect != NULL)
	{
		srect->size = fpoint_create(hfx, hfy);
		srect->color = color;
		srect->angle = RAD(rand() % 360);
		return (shape_alloc(SHAPE_RECT, srect, x, y));
	}
	return (NULL);
}

t_frect		srect_tofrect(t_srect srect, double ox, double oy)
{
	t_frect result;

	result.halfsize = srect.size;
	result.origin = fpoint_create(ox, oy);
	return (result);
}
