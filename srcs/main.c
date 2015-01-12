#include "qtree.h"
#include "sdlhandler.h"
#include "events.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define RAD(x) ((x) * M_PI / 180.0)

#define DISK_RADIUS 200.0
#define WORLD_SIZE 10000

double	vec2d_len(double x, double y)
{
	return sqrt((x * x) + (y * y));
}

double	point_len(t_fpoint pt)
{
	return sqrt((pt.x * pt.x) + (pt.y * pt.y));
}

t_fpoint point_sub(t_fpoint a, t_fpoint b)
{
	t_fpoint res;

	res.x = a.x - b.x;
	res.y = a.y - b.y;
	return (res);
}

t_fpoint point_add(t_fpoint a, t_fpoint b)
{
	t_fpoint res;

	res.x = a.x + b.x;
	res.y = a.y + b.y;
	return (res);
}

typedef struct	s_disk
{
	double		size;
	double		angle;
	uint32_t	color;
}				t_disk;

t_disk*		disk_create(double size, uint32_t color)
{
	t_disk*		disk;

	disk = (t_disk*)malloc(sizeof(t_disk));
	if (disk != NULL)
		disk->size = size;
		disk->color = color;
		disk->angle = RAD(rand() % 360);
	return (disk);
}

void		colfunc(t_qtpoint* pt, void* data)
{
	t_disk* disk;

	(void)data;
	disk = pt->data;
	if (disk->color < 0xFFFFFF)
		disk->color += 0x1;
}

bool		disk_intersect(const t_disk* disk1, const t_fpoint* pos1, const t_disk* disk2, const t_fpoint* pos2)
{
	double lenght;


	if (disk1 == disk2)
		return (false);
	lenght = point_len(point_sub(*pos1, *pos2));
	if (lenght < disk1->size + disk2->size)
		return (true);
	return (false);
}

bool	_func(t_qtpoint* point)
{
	(void)point;
	return (!(rand() % 100));
}

static void		drawdisk(t_sdlh* sdlh, t_disk* disk, double x, double y, double zoom)
{
	int ix;
	int iy;
	double radius;

	radius = disk->size * zoom;
	ix = 0;
	while (ix < radius * 2)
	{
		iy = 0;
		while (iy < radius * 2)
		{
			if (vec2d_len(ix - radius, iy - radius) < radius)
				sdlh_mixpixel(sdlh, x + ix - radius, y + iy - radius, disk->color, 0.5);
			iy++;
		}
		ix++;
	}
}

void	drawtree(t_sdlh* sdlh, const t_qtree* tree, t_fpoint* campos, t_fpoint* winoff, double zoom, bool drawgrid)
{
	int		i;
	const t_qtpoint* point;
	double ox, oy, top, bot, right, left, hfx, hfy;

	ox = (tree->bounds.origin.x * zoom) + (campos->x * zoom) + winoff->x;
	oy = (tree->bounds.origin.y * zoom) + (campos->y * zoom) + winoff->y;
	hfx = tree->bounds.halfsize.x * zoom;
	hfy = tree->bounds.halfsize.y * zoom;
	right = ox + hfx - 1.0;
	left = ox - hfx;
	bot = oy + hfy;
	top = oy - hfy - 1.0;
	if (drawgrid)
	{
		i = 0;
		while (i < hfx - 1.0)
		{
			sdlh_putpixel(sdlh->surface, ox + i, top, 0xFFFFFF);
			sdlh_putpixel(sdlh->surface, ox - i, top, 0xFFFFFF);
			sdlh_putpixel(sdlh->surface, ox + i, bot, 0xFFFFFF);
			sdlh_putpixel(sdlh->surface, ox - i, bot, 0xFFFFFF);
			i++;
		}
		i = 0;
		while (i < hfy - 1.0)
		{
			sdlh_putpixel(sdlh->surface, right, oy + i, 0xFFFFFF);
			sdlh_putpixel(sdlh->surface, right, oy - i, 0xFFFFFF);
			sdlh_putpixel(sdlh->surface, left, oy + i, 0xFFFFFF);
			sdlh_putpixel(sdlh->surface, left, oy - i, 0xFFFFFF);
			i++;
		}
	}
	if (tree->northwest != NULL)
	{
		drawtree(sdlh, tree->northwest, campos, winoff, zoom, drawgrid);
		drawtree(sdlh, tree->northeast, campos, winoff, zoom, drawgrid);
		drawtree(sdlh, tree->southwest, campos, winoff, zoom, drawgrid);
		drawtree(sdlh, tree->southeast, campos, winoff, zoom, drawgrid);
	}
	else
	{
		i = 0;
		while (i < tree->ptscount)
		{
			point = tree->points + i;
			ox = (point->pos.x * zoom) + (campos->x * zoom) + winoff->x;
			oy = (point->pos.y * zoom) + (campos->y * zoom) + winoff->y;
			drawdisk(sdlh, point->data, ox, oy, zoom);
			sdlh_putpixel(sdlh->surface, ox, oy, 0xFFFF00);
			i++;
		}
	}
}

bool	tree_intersectdisk(const t_qtree* tree, const t_disk* disk, const t_fpoint* pos, double radmax)
{
	t_array*	array;
	t_frect		range;
	unsigned int			i;
	t_qtpoint*	otherpoint;
	t_disk*		otherdisk;

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

void		billiardfunc(t_qtpoint* pt, void* data)
{
	t_fpoint	unitvec;
	t_fpoint	newpos;
	t_disk*		disk;
	t_qtree*	tree;

	tree = data;
	disk = pt->data;
	unitvec.x = cos(disk->angle) * 10;
	unitvec.y = sin(disk->angle) * 10;
	newpos = point_add(pt->pos, unitvec);
	if (newpos.x - disk->size <= -WORLD_SIZE || newpos.x + disk->size > WORLD_SIZE || newpos.y - disk->size <= -WORLD_SIZE || newpos.y + disk->size > WORLD_SIZE)
		return ;
	if (!tree_intersectdisk(tree, disk, &newpos, DISK_RADIUS))
		pt->pos = newpos;
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
				qtree_applyfunc(tree, colfunc, NULL);
				qtree_movepoints(tree, billiardfunc, tree);
				if (tree_intersect(tree, tree))
				{
					printf("FAILED\n");
					events->play = false;
				}
				if (lastptscount >= 20000)
					up = false;
			}
			else
			{
				array = qtree_removepointif(tree, _func);
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
