#ifndef TREERENDERER_H
# define TREERENDERER_H

# include <stdbool.h>
# include "qtree.h"
# include "sdlhandler.h"
# include "point.h"

void	drawtree(t_sdlh* sdlh, const t_qtree* tree, t_fpoint* campos, t_fpoint* winoff, double zoom, bool drawgrid);

#endif
