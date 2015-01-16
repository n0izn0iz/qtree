#ifndef DISK_H
#define DISK_H

#include <stdint.h>
#include <stdbool.h>
#include "point.h"
#include "shape.h"

typedef struct	s_disk
{
	double		size;
	double		angle;
	uint32_t	color;
}				t_disk;


t_shape*	disk_create(double size, uint32_t color, double x, double y);
bool		disk_intersect(const t_disk* disk1, const t_fpoint* pos1, const t_disk* disk2, const t_fpoint* pos2);

#endif

