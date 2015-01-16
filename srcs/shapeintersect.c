#include "shape.h"

#include <stdlib.h>
#include "shapetype.h"
#include "disk.h"

t_shape*		shape_intersect(const t_shape* shape1, const t_shape* shape2)
{
	t_shape* result;

	result = shape_alloc(SHAPE_UNKNOWN, NULL, 0, 0);
	if (shape1->type == SHAPE_DISK && shape2->type == SHAPE_DISK
	&& disk_intersect(shape1->data, &shape1->pos, shape2->data, &shape2->pos))
		return (result);
	shape_destroy(&result);
	return (NULL);
}

