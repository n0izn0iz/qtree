#ifndef RECT_H
# define RECT_H

#include <stdbool.h>
#include "point.h"

typedef struct	s_irect
{
	t_ipoint	origin;
	t_ipoint	halfsize;
}				t_irect;

typedef struct	s_frect
{
	t_fpoint	origin;
	t_fpoint	halfsize;
}				t_frect;

t_irect		irect_create(t_ipoint origin, t_ipoint halfsize);
t_irect*	irect_alloc(t_ipoint origin, t_ipoint halfsize);
bool		irect_containsipoint(const t_irect* rect, const t_ipoint* point);
bool		irect_intersect(const t_irect* a_rect, const t_irect* b_rect);

t_frect		frect_create(t_fpoint origin, t_fpoint halfsize);
t_frect*	frect_alloc(t_fpoint origin, t_fpoint halfsize);
bool		frect_containsfpoint(const t_frect* rect, const t_fpoint* point);
bool		frect_intersect(const t_frect* a_rect, const t_frect* b_rect);

#endif
