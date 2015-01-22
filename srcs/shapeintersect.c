#include "shape.h"

#include <stdlib.h>
#include "shapetype.h"
#include "srect.h"
#include "disk.h"
#include "diskrectintersect.h"
#include <assert.h>

t_shape*		shape_intersect(const t_shape* shape1, const t_shape* shape2)
{
	t_shape* result;
	t_frect		frect1;
	t_frect		frect2;

	result = shape_alloc(SHAPE_UNKNOWN, NULL, 0, 0);
	if (shape1->type == SHAPE_DISK && shape2->type == SHAPE_DISK
	&& disk_intersect(shape1->data, &shape1->pos, shape2->data, &shape2->pos))
		return (result);
	if (shape1->type == SHAPE_RECT && shape2->type == SHAPE_RECT)
	{
		frect1 = srect_tofrect(*(t_srect*)shape1->data, shape1->pos.x, shape1->pos.y);
		frect2 = srect_tofrect(*(t_srect*)shape2->data, shape2->pos.x, shape2->pos.y);
		if (frect_intersect(&frect1, &frect2))
			return (result);
	}
	if (shape1->type == SHAPE_RECT && shape2->type == SHAPE_DISK)
	{
		frect1 = srect_tofrect(*(t_srect*)shape1->data, shape1->pos.x, shape1->pos.y);
		if (diskrectintersect(&frect1, shape2->data, &shape2->pos))
			return (result);
	}
	if (shape2->type == SHAPE_RECT && shape1->type == SHAPE_DISK)
	{
		frect2 = srect_tofrect(*(t_srect*)shape2->data, shape2->pos.x, shape2->pos.y);
		if (diskrectintersect(&frect2, shape1->data, &shape1->pos))
			return (result);
	}
	shape_destroy(&result);
	return (NULL);
}

t_shape*		shape_collide(const t_shape* shape1, const t_shape* shape2, t_fpoint* newpos)
{
	t_shape* result;
	t_frect		frect1;
	t_frect		frect2;

	result = shape_alloc(SHAPE_UNKNOWN, NULL, 0, 0);
	if (shape1->type == SHAPE_DISK && shape2->type == SHAPE_DISK
	&& disk_collide(shape1->data, &shape1->pos, shape2->data, &shape2->pos, newpos))
		return (result);
	if (shape1->type == SHAPE_RECT && shape2->type == SHAPE_RECT)
	{
		frect1 = srect_tofrect(*(t_srect*)shape1->data, shape1->pos.x, shape1->pos.y);
		frect2 = srect_tofrect(*(t_srect*)shape2->data, shape2->pos.x, shape2->pos.y);
		if (frect_collide(&frect1, &frect2, newpos))
			return (result);
	}
	if (shape1->type == SHAPE_RECT && shape2->type == SHAPE_DISK)
	{
		frect1 = srect_tofrect(*(t_srect*)shape1->data, shape1->pos.x, shape1->pos.y);
		if (diskrectcollide(&frect1, shape2->data, &shape2->pos, newpos))
			return (result);
	}
	if (shape2->type == SHAPE_RECT && shape1->type == SHAPE_DISK)
	{
		/*frect2 = srect_tofrect(*(t_srect*)shape2->data, shape2->pos.x, shape2->pos.y);
		if (rectdiskcollide(&frect2, shape1->data, &shape1->pos, newpos))
			return (result);*/
		assert("FAIL" == NULL);
	}
	shape_destroy(&result);
	return (NULL);
}
