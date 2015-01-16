#include "treerenderer.h"

#include "diskrenderer.h"
#include "srect.h"
#include "shapetype.h"

void	drawfrect(t_sdlh* sdlh, t_frect rect, const t_fpoint* campos, const t_fpoint* winoff, double zoom, uint32_t color, bool fill)
{
	int i, j;
	double ox, oy, top, bot, right, left, hfx, hfy;

	ox = (rect.origin.x * zoom) - (campos->x * zoom) + winoff->x;
	oy = (rect.origin.y * zoom) - (campos->y * zoom) + winoff->y;
	hfx = rect.halfsize.x * zoom;
	hfy = rect.halfsize.y * zoom;
	right = ox + hfx - 1.0;
	left = ox - hfx;
	bot = oy + hfy;
	top = oy - hfy - 1.0;
	if (fill == false)
	{
		i = 0;
		while (i < hfx - 1.0)
		{
			sdlh_putpixel(sdlh->surface, ox + i, top, color);
			sdlh_putpixel(sdlh->surface, ox - i, top, color);
			sdlh_putpixel(sdlh->surface, ox + i, bot, color);
			sdlh_putpixel(sdlh->surface, ox - i, bot, color);
			i++;
		}
		i = 0;
		while (i < hfy - 1.0)
		{
			sdlh_putpixel(sdlh->surface, right, oy + i, color);
			sdlh_putpixel(sdlh->surface, right, oy - i, color);
			sdlh_putpixel(sdlh->surface, left, oy + i, color);
			sdlh_putpixel(sdlh->surface, left, oy - i, color);
			i++;
		}
	}
	else
	{
		i = left;
		while (i < right)
		{
			j = top;
			while (j < bot)
			{
				sdlh_putpixel(sdlh->surface, i, j, color);
				j++;
			}
			i++;
		}
	}
}

void	drawtree(t_sdlh* sdlh, const t_qtree* tree, t_fpoint* campos, t_fpoint* winoff, double zoom, bool drawgrid)
{
	int		i;
	const t_qtpoint* point;
	double ox, oy;
	t_frect		screenrect;
	t_srect*		rect;

	screenrect.origin = *campos;
	screenrect.halfsize = *winoff;
	screenrect.halfsize.x /= zoom;
	screenrect.halfsize.y /= zoom;
	if (!frect_intersect(&tree->bounds, &screenrect))
		return ;
	if (drawgrid)
		drawfrect(sdlh, tree->bounds, campos, winoff, zoom, 0xFFFFFF, false);
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
			ox = (point->shape.pos.x * zoom) - (campos->x * zoom) + winoff->x;
			oy = (point->shape.pos.y * zoom) - (campos->y * zoom) + winoff->y;
			if (point->shape.type == SHAPE_DISK)
				drawdisk(sdlh, point->shape.data, ox, oy, zoom);
			else if (point->shape.type == SHAPE_RECT)
			{
				rect = point->shape.data;
				drawfrect(sdlh, srect_tofrect(*rect, point->shape.pos.x, point->shape.pos.y), campos, winoff, zoom, rect->color, true);
			}
			sdlh_putpixel(sdlh->surface, ox, oy, 0xFFFF00);
			i++;
		}
	}
}
