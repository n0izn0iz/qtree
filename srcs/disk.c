#include "disk.h"
#include <stdlib.h>
#include <time.h>
#include "vecmath.h"

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
