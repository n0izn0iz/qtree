#ifndef DISKRECTINTERSECT_H
# define DISKRECTINTERSECT_H

#include <stdbool.h>
#include "rect.h"
#include "disk.h"

bool		diskrectintersect(const t_frect* rect, const t_disk* disk, const t_fpoint* diskpos);
bool		diskrectcollide(const t_frect* rect, const t_disk* disk, const t_fpoint* diskpos, t_fpoint* newpos);

#endif
