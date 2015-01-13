#include "treerenderer.h"

# include "diskrenderer.h"

void	drawtree(t_sdlh* sdlh, const t_qtree* tree, t_fpoint* campos, t_fpoint* winoff, double zoom, bool drawgrid)
{
	int		i;
	const t_qtpoint* point;
	double ox, oy, top, bot, right, left, hfx, hfy;
	t_frect		screenrect;

	ox = (tree->bounds.origin.x * zoom) - (campos->x * zoom) + winoff->x;
	oy = (tree->bounds.origin.y * zoom) - (campos->y * zoom) + winoff->y;
	hfx = tree->bounds.halfsize.x * zoom;
	hfy = tree->bounds.halfsize.y * zoom;
	right = ox + hfx - 1.0;
	left = ox - hfx;
	bot = oy + hfy;
	top = oy - hfy - 1.0;
	screenrect.origin = *campos;
	screenrect.halfsize = *winoff;
	screenrect.halfsize.x /= zoom;
	screenrect.halfsize.y /= zoom;
	if (!frect_intersect(&tree->bounds, &screenrect))
		return ;
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
			ox = (point->pos.x * zoom) - (campos->x * zoom) + winoff->x;
			oy = (point->pos.y * zoom) - (campos->y * zoom) + winoff->y;
			drawdisk(sdlh, point->data, ox, oy, zoom);
			sdlh_putpixel(sdlh->surface, ox, oy, 0xFFFF00);
			i++;
		}
	}
}
