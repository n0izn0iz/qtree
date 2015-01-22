#include "qtree.h"
#include "sdlhandler.h"
#include "events.h"
#include "disk.h"
#include "vecmath.h"
#include "treerenderer.h"
#include "srect.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "shapetype.h"

#define DISK_RADIUS 200.0
#define WORLD_SIZE 10000

bool		colfunc(t_qtpoint* pt, void* data)
{
	t_disk* disk;

	(void)data;
	if (pt->shape.type == SHAPE_DISK)
	{
		disk = pt->shape.data;
		if (disk->color < 0xFFFFFF)
			disk->color += 0x1;
	}
	return (false);
}

bool	_func(t_qtpoint* point, void* data)
{
	(void)data;
	(void)point;
	return (!(rand() % 100));
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
	bool		result;
	t_frect		querrybounds;

	tree = data;
	if (pt->shape.type != SHAPE_DISK)
		return (true);
	disk = pt->shape.data;
	unitvec.x = cos(disk->angle) * 10;
	unitvec.y = sin(disk->angle) * 10;
	newpos = point_add(pt->shape.pos, unitvec);
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
	pt->shape.pos = newpos;
	querrybounds = frect_create(pt->shape.pos, fpoint_create(disk->size + 100, disk->size + 100));
	if ((result = qtree_colliderange(tree, pt, &querrybounds, &newpos)) != false)
	{
		disk->angle = disk->angle - RAD(180.0);
		pt->shape.pos = newpos;
	}
	return (true);
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
	t_shape*		shape;
	t_qtreefunc		func[1];
	unsigned int	i;
	int				lastptscount;
	int				currptscount;
	bool			prevented;
	bool			up;
	t_array*		array;
	t_shape*		tmpshape;
	double			x, y;
	void*			shapedata;

	srand(time(NULL));
	bounds.origin.x = 0;
	bounds.origin.y = 0;
	winoff.x = WIN_WIDTH / 2.0;
	winoff.y = WIN_HEIGHT / 2.0;
	campos = bounds.origin;
	qtpoint.data = NULL;
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
				x = (rand() % WORLD_SIZE * 20) / 10.0 - WORLD_SIZE;
				y = (rand() % WORLD_SIZE * 20) / 10.0 - WORLD_SIZE;
				if (rand() % 2)
					shape = disk_create(((rand() % 1900) + 100) / 10.0, rand() % 0xFFFFFF, x, y);
				else
					shape = srect_create(((rand() % 1000) + 1000) / 10.0, ((rand() % 1000) + 1000) / 10.0, rand() % 0xFFFFFF, x, y);
				qtpoint.shape = *shape;
				if (qtpoint.shape.type == SHAPE_DISK && (qtpoint.shape.pos.x - ((t_disk*)shape->data)->size <= -WORLD_SIZE || qtpoint.shape.pos.x + ((t_disk*)shape->data)->size > WORLD_SIZE || qtpoint.shape.pos.y - ((t_disk*)shape->data)->size <= -WORLD_SIZE || qtpoint.shape.pos.y + ((t_disk*)shape->data)->size > WORLD_SIZE))
				{
					prevented = true;
					shape_destroy(&shape);
				}
				else if ((tmpshape = qtree_intersectrange(tree, &qtpoint, NULL)) != NULL)
				{
					prevented = true;
					shape_destroy(&shape);
					shape_destroy(&tmpshape);
				}
				else if (!qtree_insert(tree, &qtpoint))
				{
					prevented = true;
					shape_destroy(&shape);
					shape_destroy(&tmpshape);
				}
				else
				{
					shape_destroy(&tmpshape);
					free(shape);
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
					shapedata = qtptptr->shape.data;
					if (shapedata != NULL)
						free(shapedata);
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
