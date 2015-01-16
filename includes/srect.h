#ifndef SRECT_H
# define SRECT_H

# include <stdint.h>
# include "shape.h"
# include "rect.h"

typedef struct	s_srect
{
	t_fpoint	size;
	uint32_t	color;
	double		angle;
}				t_srect;

t_shape*	srect_create(double hfx, double hfy, uint32_t color, double x, double y);
t_frect		srect_tofrect(t_srect srect, double ox, double oy);

#endif
