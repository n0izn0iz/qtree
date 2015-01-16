#ifndef QTREE_H
# define QTREE_H

#include <stdbool.h>
#include <stdint.h>
#include "rect.h"
#include "array.h"
#include "shape.h"

#define QTREE_NODECAP 4

typedef struct	s_qtpoint
{
	t_shape		shape;
	uint32_t	type;
	void*		data;
}				t_qtpoint;

typedef struct	s_qtree
{
	t_frect				bounds;
	t_qtpoint*			points;
	int					ptscount;
	struct s_qtree*		northwest;
	struct s_qtree*		northeast;
	struct s_qtree*		southwest;
	struct s_qtree*		southeast;
}				t_qtree;

typedef bool (*t_qtreefunc)(t_qtpoint*, void*);

t_qtree*		qtree_alloc(const t_frect* bounds);
bool			qtree_insert(t_qtree* qtree, const t_qtpoint* pt);
t_shape*		qtree_intersectrange(const t_qtree* qtree, const t_qtpoint* pt, const t_frect* bounds);
t_array*		qtree_querryrange(const t_qtree* qtree, const t_frect* range);
void			qtree_applyfunc(t_qtree* qtree, t_qtreefunc* func, void* data);
void			qtree_movepoints(t_qtree* qtree, t_qtreefunc* func, void* data);
t_array*		qtree_removepointif(t_qtree* qtree, t_qtreefunc* func, void* data);
int				qtree_nodecount(const t_qtree* tree);
int				qtree_ptscount(const t_qtree* tree);
int				qtree_depth(const t_qtree* tree);

#endif
