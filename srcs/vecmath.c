#include "vecmath.h"
#include "math.h"

double	vec2d_len(double x, double y)
{
	return sqrt((x * x) + (y * y));
}

t_fpoint	angletovec(double angle)
{
	t_fpoint		result;

	result.x = cos(angle);
	result.y = sin(angle);
	return (result);
}

double	point_len(t_fpoint pt)
{
	return sqrt((pt.x * pt.x) + (pt.y * pt.y));
}

t_fpoint point_sub(t_fpoint a, t_fpoint b)
{
	t_fpoint res;

	res.x = a.x - b.x;
	res.y = a.y - b.y;
	return (res);
}

t_fpoint point_add(t_fpoint a, t_fpoint b)
{
	t_fpoint res;

	res.x = a.x + b.x;
	res.y = a.y + b.y;
	return (res);
}

