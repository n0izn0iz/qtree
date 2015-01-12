#include "qtree.h"
#include "sdlhandler.h"
#include "events.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

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

typedef struct	s_disk
{
	double		size;
	uint32_t	color;
}				t_disk;

t_disk*		disk_create(double size, uint32_t color)
{
	t_disk*		disk;

	disk = (t_disk*)malloc(sizeof(t_disk));
	if (disk != NULL)
		disk->size = size;
		disk->color = color;
	return (disk);
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

void	drawtree(t_sdlh* sdlh, const t_qtree* tree, t_fpoint* campos, t_fpoint* winoff, double zoom)
{
	int		i;
	const t_qtpoint* point;
	double ox, oy, top, bot, right, left, hfx, hfy;

	ox = (tree->bounds.origin.x * zoom) + campos->x - (winoff->x * zoom);
	oy = (tree->bounds.origin.y * zoom) + campos->y - (winoff->y * zoom);
	hfx = tree->bounds.halfsize.x * zoom;
	hfy = tree->bounds.halfsize.y * zoom;
	right = ox + hfx - 1.0;
	left = ox - hfx;
	bot = oy + hfy - 1.0;
	top = oy - hfy;
	i = 0;
	while (i < hfx)
	{
		sdlh_putpixel(sdlh->surface, ox + i, top, 0xFFFFFF);
		sdlh_putpixel(sdlh->surface, ox - i, top, 0xFFFFFF);
		sdlh_putpixel(sdlh->surface, ox + i, bot, 0xFFFFFF);
		sdlh_putpixel(sdlh->surface, ox - i, bot, 0xFFFFFF);
		i++;
	}
	i = 0;
	while (i < hfy)
	{
		sdlh_putpixel(sdlh->surface, right, oy + i, 0xFFFFFF);
		sdlh_putpixel(sdlh->surface, right, oy - i, 0xFFFFFF);
		sdlh_putpixel(sdlh->surface, left, oy + i, 0xFFFFFF);
		sdlh_putpixel(sdlh->surface, left, oy - i, 0xFFFFFF);
		i++;
	}
	if (tree->northwest != NULL)
	{
		drawtree(sdlh, tree->northwest, campos, winoff, zoom);
		drawtree(sdlh, tree->northeast, campos, winoff, zoom);
		drawtree(sdlh, tree->southwest, campos, winoff, zoom);
		drawtree(sdlh, tree->southeast, campos, winoff, zoom);
	}
	else
	{
		i = 0;
		while (i < tree->ptscount)
		{
			point = tree->points + i;
			ox = (point->pos.x * zoom) + campos->x - (winoff->x * zoom);
			oy = (point->pos.y * zoom) + campos->y - (winoff->y * zoom);
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
			return (true);
		i++;
	}
	return (false);
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
			if (tree_intersectdisk(root, point->data, &point->pos, 30.0))
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
	t_fpoint		campos;
	t_disk*			disk;
	int				lastptscount;
	int				currptscount;
	bool			prevented;

	srand(time(NULL));
	bounds.origin.x = WIN_WIDTH / 2.0;
	bounds.origin.y = WIN_HEIGHT / 2.0;
	campos = bounds.origin;
	bounds.halfsize.x = WIN_WIDTH / 2.0;
	bounds.halfsize.y = WIN_HEIGHT / 2.0;
	qtpoint.type = 0x0;
	tree = qtree_alloc(&bounds);
	sdlh_init(&sdlh);
	events = events_create();
	lastptscount = 0;
	while (events->quitflag == false)
	{
		campos.x -= events->mov_x * events->zoom * 2;
		campos.y += events->mov_y * events->zoom * 2;
		if (events->play)
		{
			prevented = false;
			printf("______________\n");
			printf("%i nodes for %i points and a depth of %i\n", qtree_nodecount(tree), lastptscount, qtree_depth(tree));
			qtpoint.pos.x = (rand() % (WIN_WIDTH * 10)) / 10.0;
			qtpoint.pos.y = (rand() % (WIN_HEIGHT * 10)) / 10.0;
			disk = disk_create(/*((rand() % 100) + 150) / 10.0*/ 25.0, rand() % 0xFFFFFF);
			qtpoint.data = disk;
			if (qtpoint.pos.x - disk->size < 0.0 || qtpoint.pos.x + disk->size >= WIN_WIDTH || qtpoint.pos.y - disk->size < 0.0 || qtpoint.pos.y + disk->size >= WIN_HEIGHT)
			{
				printf("Out of border prevented\n");
				prevented = true;
				free(disk);
			}
			else if (tree_intersectdisk(tree, disk, &qtpoint.pos, 30.0))
			{
				printf("Intersection prevented\n");
				prevented = true;
				free(disk);
			}
			else if (!qtree_insert(tree, &qtpoint))
			{
				printf("Insertion failed\n");
				prevented = true;
				free(disk);
			}
			else
				printf("Insert done\n");
			currptscount = qtree_ptscount(tree);
			if (!prevented && currptscount == lastptscount)
			{
				printf("Something went really wrong!\n");
				printf("debug: %i nodes for %i points and a depth of %i\n", qtree_nodecount(tree), qtree_ptscount(tree), qtree_depth(tree));
				events->play = false;
			}
			lastptscount = currptscount;
			if (tree_intersect(tree, tree))
			{
				printf("FAILED\n");
				events->play = false;
			}
		}
		drawtree(&sdlh, tree, &campos, &bounds.origin, events->zoom);
		sdlh_update_window(&sdlh);
		events_update(events);
	}
	return (0);
}
