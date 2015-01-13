#ifndef SHAPES_H
# define SHAPES_H

typedef struct	s_shape
{
	unsigned int	type;
	void*			data;
}				t_shape;

bool		shape_intersect(const t_shape* shape1, const t_shape* shape2, t_shape* result);

#endif
