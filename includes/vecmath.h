#ifndef VECMATH_H
#define VECMATH_H

#include "point.h"
#include <math.h>

#define RAD(x) ((x) * M_PI / 180.0)

double		vec2d_len(double x, double y);
double		point_len(t_fpoint pt);
t_fpoint	point_sub(t_fpoint a, t_fpoint b);
t_fpoint	point_add(t_fpoint a, t_fpoint b);
t_fpoint	angletovec(double angle);

#endif
