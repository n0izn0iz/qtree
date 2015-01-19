#include "diskrectintersect.h"

#include "vecmath.h"
#include <math.h>


t_frect		disktofrect(const t_disk* disk, const t_fpoint* pt)
{
	t_frect		result;

	result.origin.x = pt->x;
	result.origin.y = pt->y;
	result.halfsize.x = disk->size;
	result.halfsize.y = disk->size;
	return (result);
}

bool		diskrectintersect(const t_frect* rect, const t_disk* disk, const t_fpoint* diskpos)
{
	t_fpoint	pt;
	t_frect		diskrect;

	diskrect = disktofrect(disk, diskpos);
	pt = angletovec(RAD(45));
	(void)pt;
	return (frect_intersect(rect, &diskrect));
}
