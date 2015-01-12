#include "events.h"
#include <SDL.h>
#include <stdio.h>

t_evnh*		events_create()
{
	t_evnh*	env;

	env = malloc(sizeof(t_evnh));
	if (env != NULL)
	{
		env->quitflag = false;
		env->insertflag = false;
		SDL_GetMouseState(&env->mouse_x, &env->mouse_y);
		env->play = false;
		env->mov_x = 0;
		env->mov_y = 0;
		env->zoom = 1.0;
		env->drawgrid = true;
	}
	return (env);
}

void		events_update(t_evnh* env)
{
	SDL_Event e;
	const uint8_t* kstate;
	
	while(SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
			env->quitflag = true;
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym) {
			case SDLK_SPACE:
				env->play = !env->play;
				break ;
			case SDLK_g:
				env->drawgrid = !env->drawgrid;
				break ;
			default:
				break ;
			}
		}
	}
	kstate = SDL_GetKeyboardState(NULL);
	env->mov_x = 0;
	env->mov_y = 0;
	if (kstate[SDL_GetScancodeFromKey(SDLK_d)])
		env->mov_x += 1;
	if (kstate[SDL_GetScancodeFromKey(SDLK_q)])
		env->mov_x -= 1;
	if (kstate[SDL_GetScancodeFromKey(SDLK_z)])
		env->mov_y += 1;
	if (kstate[SDL_GetScancodeFromKey(SDLK_s)])
		env->mov_y -= 1;
	if (kstate[SDL_GetScancodeFromKey(SDLK_p)])
		env->zoom *= 1.1;
	if (kstate[SDL_GetScancodeFromKey(SDLK_m)])
		env->zoom *= 0.9;
	SDL_GetMouseState(&env->mouse_x, &env->mouse_y);
}
