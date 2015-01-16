#ifndef SHAPE_H
# define SHAPE_H

# include "point.h"

typedef struct	s_shape
{
	unsigned int	type;
	t_fpoint		pos;
	void*			data;
}				t_shape;

t_shape*		shape_alloc(unsigned int type, void* data, double x, double y);
t_shape			shape_create(unsigned int type, void* data, double x, double y);
void			shape_destroy(t_shape** shape);
t_shape*		shape_intersect(const t_shape* shape1, const t_shape* shape2);

#endif
