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

bool	shouldinsert(const t_qtree* tree, const t_qtpoint* point)
{
	double		lenght;
	t_array*	array;
	unsigned int			i;
	t_disk*		disk;
	t_qtpoint*	otherpoint;
	t_disk*		otherdisk;
	t_frect		range;

	disk = point->data;
	range.origin = point->pos;
	range.halfsize.x = disk->size + 25.0;
	range.halfsize.y = disk->size + 25.0;
	array = qtree_querryrange(tree, &range);
	i = 0;
	while (i < array->size)
	{
		otherpoint = array_get(array, i);
		otherdisk = otherpoint->data;
		lenght = vec2d_len(otherpoint->pos.x - point->pos.x, otherpoint->pos.y - point->pos.y);
		if (lenght < disk->size + otherdisk->size)
		{
			array_destroy(array);
			return (false);
		}
		i++;
	}
	array_destroy(array);
	return (true);
}

int		main(void)
{
	t_qtree*		tree;
	t_sdlh			sdlh;
	t_frect			bounds;
	t_evnh*			events;
	t_qtpoint		qtpoint;
	t_fpoint		campos;

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
	while (events->quitflag == false)
	{
		campos.x -= events->mov_x * events->zoom * 2;
		campos.y += events->mov_y * events->zoom * 2;
		qtpoint.pos.x = (rand() % (WIN_WIDTH * 10)) / 10.0;
		qtpoint.pos.y = (rand() % (WIN_HEIGHT * 10)) / 10.0;
		qtpoint.data = disk_create(((rand() % 200) + 50) / 10.0, rand() % 0xFFFFFF);
		if (!shouldinsert(tree, &qtpoint) || !qtree_insert(tree, &qtpoint))
			free(qtpoint.data);
		drawtree(&sdlh, tree, &campos, &bounds.origin, events->zoom);
		sdlh_update_window(&sdlh);
		events_update(events);
	}
	return (0);
}
