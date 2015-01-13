#ifndef RECT_H
# define RECT_H

#include <stdbool.h>
#include "point.h"

typedef struct	s_frect
{
	t_fpoint	origin;
	t_fpoint	halfsize;
}				t_frect;

t_frect		frect_create(t_fpoint origin, t_fpoint halfsize);
t_frect*	frect_alloc(t_fpoint origin, t_fpoint halfsize);
bool		frect_containsfpoint(const t_frect* rect, const t_fpoint* point);
bool		frect_intersect(const t_frect* a_rect, const t_frect* b_rect);

#endif
