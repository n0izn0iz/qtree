#include "diskrectintersect.h"

#include "vecmath.h"
#include <math.h>

static double _clamp(double value, double min, double max)
{
	if (value <= min)
		return (min);
	if (value >= max)
		return (max);
	return (value);
}


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
	t_fpoint closept;
	t_fpoint distance;
	double distancesq;
	double rectleft, rectright, rectbottom, recttop;

	rectleft = rect->origin.x - rect->halfsize.x;
	rectright = rect->origin.x + rect->halfsize.x;
	recttop = rect->origin.y - rect->halfsize.y;
	rectbottom = rect->origin.y + rect->halfsize.y;
	closept.x = _clamp(diskpos->x, rectleft, rectright);
	closept.y = _clamp(diskpos->y, recttop, rectbottom);
	distance.x = diskpos->x - closept.x;
	distance.y = diskpos->y - closept.y;
	distancesq = (distance.x * distance.x) + (distance.y * distance.y);
	if (distancesq < disk->size * disk->size)
		return (true);
	return (false);
}

bool		diskrectcollide(const t_frect* rect, const t_disk* disk, const t_fpoint* diskpos, t_fpoint* newpos)
{
	t_fpoint closept;
	t_fpoint distance;
	t_fpoint direction;
	double distancelen;
	double collision;
	double rectleft, rectright, rectbottom, recttop;

	rectleft = rect->origin.x - rect->halfsize.x;
	rectright = rect->origin.x + rect->halfsize.x;
	recttop = rect->origin.y - rect->halfsize.y;
	rectbottom = rect->origin.y + rect->halfsize.y;
	closept.x = _clamp(diskpos->x, rectleft, rectright);
	closept.y = _clamp(diskpos->y, recttop, rectbottom);
	distance.x = diskpos->x - closept.x;
	distance.y = diskpos->y - closept.y;
	distancelen = sqrt((distance.x * distance.x) + (distance.y * distance.y));
	collision = distancelen - disk->size;
	if (collision >= 0.0)
		return (false);
	*newpos = *diskpos;
	direction.x = distance.x / distancelen;
	direction.y = distance.y / distancelen;
	newpos->x += collision * direction.x;
	newpos->y += collision * direction.y;
	return (true);
}

