#ifndef FPOINT_H
# define FPOINT_H

typedef struct		s_fpoint
{
	double x;
	double y;
}					t_fpoint;

t_fpoint		fpoint_create(double x, double y);
t_fpoint*		fpoint_alloc(double x, double y);

#endif
