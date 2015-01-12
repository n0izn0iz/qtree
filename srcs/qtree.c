#include "qtree.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

t_qtree*		qtree_alloc(const t_frect* bounds)
{
	t_qtree*		tree;

	tree = (t_qtree*)malloc(sizeof(t_qtree));
	if (tree != NULL)
	{
		tree->bounds = *bounds;
		tree->ptscount = 0;
		tree->northwest = NULL;
		tree->points = (t_qtpoint*)malloc(sizeof(t_qtpoint) * QTREE_NODECAP);
	}
	return (tree);
}

static void		qtree_subdivide(t_qtree* qtree)
{
	t_frect		bounds;
	t_qtpoint*	pt;
	int			i;

	bounds.halfsize.x = qtree->bounds.halfsize.x / 2.0;
	bounds.halfsize.y = qtree->bounds.halfsize.y / 2.0;
	assert(bounds.halfsize.x >= 1.0 && bounds.halfsize.y >= 1.0);
	bounds.origin.x = qtree->bounds.origin.x - bounds.halfsize.x;
	bounds.origin.y = qtree->bounds.origin.y + bounds.halfsize.y;
	qtree->northwest = qtree_alloc(&bounds);
	bounds.origin.x = qtree->bounds.origin.x + bounds.halfsize.x;
	qtree->northeast = qtree_alloc(&bounds);
	bounds.origin.y = qtree->bounds.origin.y - bounds.halfsize.y;
	qtree->southeast = qtree_alloc(&bounds);
	bounds.origin.x = qtree->bounds.origin.x - bounds.halfsize.x;
	qtree->southwest = qtree_alloc(&bounds);
	i = 0;
	while (i < qtree->ptscount)
	{
		pt = qtree->points + i;
		i += 1;
		if (qtree_insert(qtree->northwest, pt) == true)
			continue ;
		if (qtree_insert(qtree->northeast, pt) == true)
			continue ;
		if (qtree_insert(qtree->southwest, pt) == true)
			continue ;
		if (qtree_insert(qtree->southeast, pt) == true)
			continue ;
		fprintf(stderr, "qtree failed to subdivide!\n");
		exit(-1);
	}
	qtree->ptscount = 0;
}

bool			qtree_insert(t_qtree* qtree, t_qtpoint* pt)
{
	if (frect_containsfpoint(&qtree->bounds, &pt->pos) == false)
		return (false);
	if (qtree->northwest == NULL)
	{
		if (qtree->ptscount < QTREE_NODECAP)
		{
			qtree->points[qtree->ptscount] = *pt;
			qtree->ptscount += 1;
			return (true);
		}
		qtree_subdivide(qtree);
	}
	if (qtree_insert(qtree->northwest, pt) == true)
		return (true);
	if (qtree_insert(qtree->northeast, pt) == true)
		return (true);
	if (qtree_insert(qtree->southwest, pt) == true)
		return (true);
	if (qtree_insert(qtree->southeast, pt) == true)
		return (true);
	fprintf(stderr, "qtree failed to insert!\n");
	exit(-1);
}

t_array*		qtree_querryrange(const t_qtree* qtree, const t_frect* range)
{
	t_array*		array;
	int				i;
	t_qtpoint*		point;

	array = array_create(10);
	if (frect_intersect(&qtree->bounds, range) == false)
		return (array);
	if (qtree->northwest == NULL)
	{
		i = 0;
		while (i < qtree->ptscount)
		{
			point = qtree->points + i;
			if (frect_containsfpoint(range, &point->pos))
				array_append(array, point);
			i += 1;
		}
		return (array);
	}
	array_merge(array, qtree_querryrange(qtree->northwest, range));
	array_merge(array, qtree_querryrange(qtree->northeast, range));
	array_merge(array, qtree_querryrange(qtree->southwest, range));
	array_merge(array, qtree_querryrange(qtree->southeast, range));
	return (array);
}

static void		_removepoint(t_qtpoint* array, int index, int* size)
{
	(*size) -= 1;
	array[index] = array[(*size)];
}

static int		_childspointcount(const t_qtree* qtree)
{
	return (qtree->northwest->ptscount +
			qtree->northeast->ptscount +
			qtree->southwest->ptscount +
			qtree->southeast->ptscount);
}

static bool		qtree_ispreleaf(const t_qtree* qtree)
{
	if (qtree->northwest->northwest != NULL)
		return (false);
	if (qtree->northeast->northwest != NULL)
		return (false);
	if (qtree->southwest->northwest != NULL)
		return (false);
	if (qtree->southeast->northwest != NULL)
		return (false);
	return (true);
}

static void		qtree_mergenode(t_qtree* qtree)
{
	t_qtree*	node;
	int			i;

	assert(qtree->northwest != NULL);
	node = qtree->northwest;
	assert(node->northwest == NULL);
	qtree->northwest = NULL;
	i = -1;
	while (++i < node->ptscount)
		assert(qtree_insert(qtree, node->points + i) == true);
	free(node->points);
	free(node);
	node = qtree->northeast;
	assert(node->northwest == NULL);
	i = -1;
	while (++i < node->ptscount)
		assert(qtree_insert(qtree, node->points + i) == true);
	free(node->points);
	free(node);
	node = qtree->southwest;
	assert(node->northwest == NULL);
	i = -1;
	while (++i < node->ptscount)
		assert(qtree_insert(qtree, node->points + i) == true);
	free(node->points);
	free(node);
	node = qtree->southeast;
	assert(node->northwest == NULL);
	i = -1;
	while (++i < node->ptscount)
		assert(qtree_insert(qtree, node->points + i) == true);
	free(node->points);
	free(node);
}

t_array*		qtree_removepointif(t_qtree* qtree, bool (*func)(t_qtpoint*))
{
	t_array* 	points;
	t_qtpoint*	currpt;
	int i;

	points = array_create(10);
	if (qtree->northwest == NULL)
	{
		i = 0;
		while (i < qtree->ptscount)
		{
			currpt = (t_qtpoint*)malloc(sizeof(t_qtpoint));
			*currpt = qtree->points[i];
			if (func(currpt))
			{
				_removepoint(qtree->points, i, &qtree->ptscount);
				array_append(points, currpt);
			}
			else
				i++;
		}
	}
	else
	{
		array_merge(points, qtree_removepointif(qtree->northwest, func));
		array_merge(points, qtree_removepointif(qtree->northeast, func));
		array_merge(points, qtree_removepointif(qtree->southwest, func));
		array_merge(points, qtree_removepointif(qtree->southeast, func));
		if (_childspointcount(qtree) <= QTREE_NODECAP && qtree_ispreleaf(qtree))
			qtree_mergenode(qtree);
	}
	return (points);
}

int				qtree_nodecount(const t_qtree* tree)
{
	if (tree->northwest == NULL)
		return (1);
	else
	{
		return (qtree_nodecount(tree->northwest)
				+ qtree_nodecount(tree->northeast)
				+ qtree_nodecount(tree->southwest)
				+ qtree_nodecount(tree->southeast) + 1);
	}
}

int				qtree_ptscount(const t_qtree* tree)
{
	if (tree->northwest == NULL)
		return (tree->ptscount);
	else
	{
		return (qtree_ptscount(tree->northwest)
				+ qtree_ptscount(tree->northeast)
				+ qtree_ptscount(tree->southwest)
				+ qtree_ptscount(tree->southeast));
	}
}

int				qtree_depth(const t_qtree* tree)
{
	int result;
	int curr;

	if (tree->northwest == NULL)
		return (0);
	else
	{
		result = qtree_depth(tree->northwest);
		curr = qtree_depth(tree->northeast);
		result = curr > result ? curr : result;
		curr = qtree_depth(tree->southwest);
		result = curr > result ? curr : result;
		curr = qtree_depth(tree->southeast);
		result = curr > result ? curr : result;
	}
	return (result + 1);
}

void			qtree_applyfunc(t_qtree* qtree, void (*func)(t_qtpoint*, void*), void* data)
{
	int i;

	if (qtree->northwest != NULL)
	{
		qtree_applyfunc(qtree->northwest, func, data);
		qtree_applyfunc(qtree->northeast, func, data);
		qtree_applyfunc(qtree->southwest, func, data);
		qtree_applyfunc(qtree->southeast, func, data);
	}
	else
	{
		i = 0;
		while (i < qtree->ptscount)
		{
			func(qtree->points + i, data);
			i++;
		}
	}
}


void			_qtree_movepoints(t_qtree* qtree, void (*func)(t_qtpoint*, void*), void* data, t_qtree* root)
{
	int			i;
	t_qtpoint	pt;

	if (qtree->northwest != NULL)
	{
		_qtree_movepoints(qtree->northwest, func, data, root);
		_qtree_movepoints(qtree->northeast, func, data, root);
		_qtree_movepoints(qtree->southwest, func, data, root);
		_qtree_movepoints(qtree->southeast, func, data, root);
		if (_childspointcount(qtree) <= QTREE_NODECAP && qtree_ispreleaf(qtree))
			qtree_mergenode(qtree);
	}
	else
	{
		i = 0;
		while (i < qtree->ptscount)
		{
			pt = qtree->points[i];
			func(&pt, data);
			if (!frect_containsfpoint(&qtree->bounds, &pt.pos))
			{
				_removepoint(qtree->points, i, &qtree->ptscount);
				assert(qtree_insert(root, &pt));
			}
			else
				qtree->points[i] = pt;
			i++;
		}
	}
}

void			qtree_movepoints(t_qtree* qtree, void (*func)(t_qtpoint*, void*), void* data)
{
	_qtree_movepoints(qtree, func, data, qtree);
}

