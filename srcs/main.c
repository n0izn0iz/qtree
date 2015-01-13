#include "qtree.h"
#include "sdlhandler.h"
#include "events.h"
#include "disk.h"
#include "vecmath.h"
#include "treerenderer.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define DISK_RADIUS 200.0
#define WORLD_SIZE 10000

bool		colfunc(t_qtpoint* pt, void* data)
{
	t_disk* disk;

	(void)data;
	disk = pt->data;
	if (disk->color < 0xFFFFFF)
		disk->color += 0x1;
	return (false);
}

bool	_func(t_qtpoint* point, void* data)
{
	(void)data;
	(void)point;
	return (!(rand() % 100));
}

bool	tree_intersectdisk(const t_qtree* tree, const t_disk* disk, const t_fpoint* pos, double radmax)
{
	t_array*		array;
	t_frect			range;
	unsigned int	i;
	t_qtpoint*		otherpoint;
	t_disk*			otherdisk;

	range.origin = *pos;
	range.halfsize.x = disk->size + radmax;
	range.halfsize.y = disk->size + radmax;
	array = qtree_querryrange(tree, &range);
	i = 0;
	while (i < array->size)
	{
		otherpoint = array_get(array, i);
		otherdisk = otherpoint->data;
		if (disk_intersect(disk, pos, otherdisk, &otherpoint->pos))
		{
			array_destroy(array);
			return (true);
		}
		i++;
	}
	array_destroy(array);
	return (false);
}

t_fpoint	angletovec(double angle)
{
	t_fpoint		result;

	result.x = cos(angle);
	result.y = sin(angle);
	return (result);
}

double		reflect(double angle, t_fpoint* normal)
{
	/*t_fpoint		bvec;
	t_fpoint		cvec;
	*/double/*			b, c, */result;
	/*
	cvec = angletovec(angle);
	bvec.x = cvec.x * normal->x;
	bvec.y = cvec.y * normal->y;
	b = point_len(bvec);
	c = point_len(cvec);
	result = -acos(b / c);*/
	result = angle - RAD(180.0);
	(void)normal;
	return (result);
}

bool		billiardfunc(t_qtpoint* pt, void* data)
{
	t_fpoint	unitvec;
	t_fpoint	newpos;
	t_disk*		disk;
	t_qtree*	tree;
	t_fpoint	normal;

	tree = data;
	disk = pt->data;
	unitvec.x = cos(disk->angle) * 10;
	unitvec.y = sin(disk->angle) * 10;
	newpos = point_add(pt->pos, unitvec);
	if (newpos.x - disk->size <= -WORLD_SIZE)
	{
		normal.x = 1;
		normal.y = 0;
		disk->angle = reflect(disk->angle, &normal);
		return (true);
	}
	if (newpos.x + disk->size > WORLD_SIZE)
	{
		normal.x = -1;
		normal.y = 0;
		disk->angle = reflect(disk->angle, &normal);
		return (true);
	}
	if (newpos.y - disk->size <= -WORLD_SIZE) 
	{
		normal.x = 0;
		normal.y = 1;
		disk->angle = reflect(disk->angle, &normal);
		return (true);
	}
	if (newpos.y + disk->size > WORLD_SIZE)
	{
		normal.x = 0;
		normal.y = -1;
		disk->angle = reflect(disk->angle, &normal);
		return (true);
	}
	if (!tree_intersectdisk(tree, disk, &newpos, DISK_RADIUS))
		pt->pos = newpos;
	else
		disk->angle = disk->angle - RAD(180.0);
	return (true);
}


bool	tree_intersect(const t_qtree* tree, const t_qtree* root)
{
	int i;
	const t_qtpoint*	point;

	if (tree->northwest == NULL)
	{
		i = 0;
		while (i < tree->ptscount)
		{
			point = tree->points + i;
			if (tree_intersectdisk(root, point->data, &point->pos, DISK_RADIUS))
				return (true);
			i++;
		}
	}
	else
	{
		if (tree_intersect(tree->northwest, root) == true)
			return (true);
		if (tree_intersect(tree->northeast, root) == true)
			return (true);
		if (tree_intersect(tree->southwest, root) == true)
			return (true);
		if (tree_intersect(tree->southeast, root) == true)
			return (true);
	}
	return (false);
}

int		main(void)
{
	t_qtree*		tree;
	t_sdlh			sdlh;
	t_frect			bounds;
	t_evnh*			events;
	t_qtpoint		qtpoint;
	t_qtpoint*		qtptptr;
	t_fpoint		campos;
	t_fpoint		winoff;
	t_disk*			disk;
	t_qtreefunc		func[1];
	unsigned int	i;
	int				lastptscount;
	int				currptscount;
	bool			prevented;
	bool			up;
	t_array*		array;

	srand(time(NULL));
	bounds.origin.x = 0;
	bounds.origin.y = 0;
	winoff.x = WIN_WIDTH / 2.0;
	winoff.y = WIN_HEIGHT / 2.0;
	campos = bounds.origin;
	qtpoint.pos.x = 0;
	qtpoint.pos.y = 0;
	bounds.halfsize.x = WORLD_SIZE;
	bounds.halfsize.y = WORLD_SIZE;
	qtpoint.type = 0x0;
	tree = qtree_alloc(&bounds);
	sdlh_init(&sdlh);
	events = events_create();
	lastptscount = 0;
	up = true;
	while (events->quitflag == false)
	{
		campos.x -= (events->mov_x / events->zoom * 10);
		campos.y += (events->mov_y / events->zoom * 10);
		if (events->play)
		{
			printf("______________\n");
			printf("%i nodes for %i points and a depth of %i\n", qtree_nodecount(tree), lastptscount, qtree_depth(tree));
			if (up)
			{
				prevented = false;
				qtpoint.pos.x = (rand() % WORLD_SIZE * 20) / 10.0 - WORLD_SIZE;
				qtpoint.pos.y = (rand() % WORLD_SIZE * 20) / 10.0 - WORLD_SIZE;
				disk = disk_create(((rand() % 1900) + 100) / 10.0, rand() % 0xFFFFFF);
				qtpoint.data = disk;
				if (qtpoint.pos.x - disk->size <= -WORLD_SIZE || qtpoint.pos.x + disk->size > WORLD_SIZE || qtpoint.pos.y - disk->size <= -WORLD_SIZE || qtpoint.pos.y + disk->size > WORLD_SIZE)
				{
					prevented = true;
					free(disk);
				}
				else if (tree_intersectdisk(tree, disk, &qtpoint.pos, DISK_RADIUS))
				{
					prevented = true;
					free(disk);
				}
				else if (!qtree_insert(tree, &qtpoint))
				{
					prevented = true;
					free(disk);
				}
				currptscount = qtree_ptscount(tree);
				if (!prevented && currptscount == lastptscount)
				{
					printf("Something went really wrong!\n");
					printf("debug: %i nodes for %i points and a depth of %i\n", qtree_nodecount(tree), qtree_ptscount(tree), qtree_depth(tree));
					events->play = false;
				}
				lastptscount = currptscount;
				func[0] = colfunc;
				qtree_applyfunc(tree, func, NULL);
				func[0] = billiardfunc;
				qtree_movepoints(tree, func, tree);
				if (tree_intersect(tree, tree))
				{
					printf("FAILED\n");
					events->play = false;
				}
				if (lastptscount >= 2000)
					up = false;
			}
			else
			{
				func[0] = _func;
				array = qtree_removepointif(tree, func, NULL);
				lastptscount = qtree_ptscount(tree);
				i = 0;
				while (i < array->size)
				{
					qtptptr = array_get(array, i);
					free(qtptptr->data);
					free(qtptptr);
					i++;
				}
				array_destroy(array);
				if (lastptscount <= 0)
					up = true;
			}
		}
		drawtree(&sdlh, tree, &campos, &winoff, events->zoom, events->drawgrid);
		sdlh_update_window(&sdlh);
		events_update(events);
	}
	return (0);
}
