#include "shape.h"

#include <stdlib.h>

t_shape*	shape_alloc(unsigned int type, void* data, double x, double y)
{
	t_shape*	shape;

	shape = (t_shape*)malloc(sizeof(t_shape));
	if (shape != NULL)
	{
		shape->type = type;
		shape->data = data;
		shape->pos.x = x;
		shape->pos.y = y;
	}
	return (shape);
}


t_shape			shape_create(unsigned int type, void* data, double x, double y)
{
	t_shape shape;

	shape.type = type;
	shape.data = data;
	shape.pos.x = x;
	shape.pos.y = y;
	return (shape);
}

