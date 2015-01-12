#ifndef EVENTS_H
# define EVENTS_H

#include <stdbool.h>

typedef struct		s_evnh
{
	bool			quitflag;
	bool			insertflag;
	bool			play;
	int				mouse_x;
	int				mouse_y;
	int				mov_x;
	int				mov_y;
	double			zoom;
}					t_evnh;


t_evnh*		events_create();
void		events_update(t_evnh* env);

#endif
