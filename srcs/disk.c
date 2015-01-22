#include "disk.h"
#include <stdlib.h>
#include <time.h>
#include "vecmath.h"
#include "shapetype.h"
#include "shape.h"

t_shape*		disk_create(double size, uint32_t color, double x, double y)
{
	t_disk*		disk;

	disk = (t_disk*)malloc(sizeof(t_disk));
	if (disk != NULL)
	{
		disk->size = size;
		disk->color = color;
		disk->angle = RAD(rand() % 360);
		return (shape_alloc(SHAPE_DISK, disk, x, y));
	}
	return (NULL);
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

bool		disk_collide(const t_disk* disk1, const t_fpoint* pos1, const t_disk* disk2, const t_fpoint* pos2, t_fpoint* newpos)
{
	double		lenght;
	double		distance;
	t_fpoint	vec;
	t_fpoint	unitvec;

	if (disk1 == disk2)
		return (false);
	vec = point_sub(*pos1, *pos2);
	lenght = point_len(vec);
	distance = lenght - disk1->size + disk2->size;
	if (distance >= 0)
		return (false);
	unitvec.x = vec.x / lenght;
	unitvec.y = vec.y / lenght;
	newpos->x = pos1->x + (unitvec.x * distance);
	newpos->y = pos1->y + (unitvec.y * distance);
	return (true);
}

