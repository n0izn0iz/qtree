#include "diskrenderer.h"

#include "vecmath.h"

void		drawdisk(t_sdlh* sdlh, t_disk* disk, double x, double y, double zoom)
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
