#include "shape.h"

#include <stdlib.h>

void			shape_destroy(t_shape** shape)
{
	if (*shape == NULL)
		return ;
	if ((*shape)->data != NULL)
		free((*shape)->data);
	free(*shape);
	*shape = NULL;
}
