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
	i = 0;
	while (i < qtree->ptscount)
	{
		point = qtree->points + i;
		if (frect_containsfpoint(range, &point->pos))
			array_append(array, point);
		i += 1;
	}
	if (qtree->northwest == NULL)
		return (array);
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

