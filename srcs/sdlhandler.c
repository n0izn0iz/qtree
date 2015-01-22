#include <stdio.h>

#include "strjoin.h"
#include "sdlhandler.h"
#include "xp_sleep.h"

#define LAST(k, n) ((k) & ((1<<(n))-1))
#define MID(k, m, n) LAST((k)>>(m),((n)-(m)))
#define SPRITES_DIR "sprites/"

static void		sdl_error(const char* str)
{
	fprintf(stderr, "%s: %s\n", str, SDL_GetError());
	exit(-1);
}

uint32_t		colmix(uint32_t cola, uint32_t colb, float gain)
{
	int rr;
	int gr;
	int br;

	rr = (MID(cola, 16, 24) * (1 - gain)) + (MID(colb, 16, 24) * gain);
	gr = (MID(cola, 8, 16) * (1 - gain)) + (MID(colb, 8, 16) * gain);
	br = (MID(cola, 0, 8) * (1 - gain)) + (MID(colb, 0, 8) * gain);
	return ((rr << 16) + (gr << 8) + br);
}

static void		displayrendererinfo(SDL_Renderer*	renderer, int drivernum)
{
	SDL_RendererInfo	info;

	if (renderer != NULL)
		SDL_GetRendererInfo(renderer, &info);
	else
		SDL_GetRenderDriverInfo(drivernum, &info);
	printf("Name: %s\n", info.name);
	/*print_renflag(info.flags);*/
	printf("Max texture width: %i, height: %i\n", info.max_texture_width, info.max_texture_height);
	printf("%i supported texture formats\n", info.num_texture_formats);
}

void			sdlh_init(t_sdlh*	env)
{
	if (SDL_Init(SDL_INIT_VIDEO))
		sdl_error("Unable to initialize SDL: %s\n");
	atexit(SDL_Quit);
	if ((env->window = SDL_CreateWindow("factoria", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_RESIZABLE)) == NULL)
		sdl_error("Unable to create window: %s\n");
	if ((env->renderer = SDL_CreateRenderer(env->window, -1, SDL_RENDERER_PRESENTVSYNC)) == NULL)
		sdl_error("Unable to create renderer: %s\n");
	printf ("Bound render driver:\n");
	displayrendererinfo(env->renderer, 0);
	if ((env->texture = SDL_CreateTexture(env->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, WIN_WIDTH, WIN_HEIGHT)) == NULL)
		sdl_error("Unable to create Texture: %s\n");
	env->surface = SDL_CreateRGBSurfaceFrom(env->pixels, WIN_WIDTH, WIN_HEIGHT, 32, WIN_WIDTH * sizeof(uint32_t), 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	if (env->surface == NULL)
		printf("ERROR: %s\n", SDL_GetError());
}

void			sdlh_putpixel(SDL_Surface* dest, int x, int y, uint32_t pixel)
{
	if (y < dest->h && x < dest->w && y >= 0 && x >= 0)
		((uint32_t*)dest->pixels)[y * WIN_WIDTH + x] = pixel;
}

/*SDL_Surface*	sdlh_loadandconvert(char *name)
{
	SDL_Surface*	image;
	SDL_Surface*	conversion;
	char*			tmp;

	tmp = strjoin(SPRITES_DIR, name);
	image = IMG_Load(tmp);
	free(tmp);
	if (!image)
		printf("ERROR: %s\n", IMG_GetError());
	conversion = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_ARGB8888, 0);
	SDL_FreeSurface(image);
	return (conversion);
}*/

void			sdlh_mixpixel(t_sdlh *sdlh, int x, int y, uint32_t pixel, float gain)
{
	uint32_t	*currpixel;

	if (y >= WIN_HEIGHT || x >= WIN_WIDTH || x < 0 || y < 0)
		return ;
	currpixel = sdlh->pixels + (y * WIN_WIDTH + x);
	*currpixel = colmix(pixel, *currpixel, gain);
}

void			sdlh_update_window(const t_sdlh*	env)
{
	static int	lastframe = 0;
	const int	framerate = 15;
	int			now;
	int x, y;

	SDL_UpdateTexture(env->texture, NULL, env->pixels, WIN_WIDTH * sizeof(uint32_t));
	SDL_RenderCopy(env->renderer, env->texture, NULL, NULL);
	SDL_RenderPresent(env->renderer);
	SDL_RenderClear(env->renderer);
	x = 0;
	while (x < WIN_WIDTH)
	{
		y = 0;
		while (y < WIN_HEIGHT)
		{
			sdlh_putpixel(env->surface, x, y, 0x0);
			y++;
		}
		x++;
	}
	now = SDL_GetTicks();
	if (now < lastframe + framerate)
		Sleep(lastframe + framerate - now);
	lastframe = lastframe + framerate;
}

void			sdlh_cleanup(t_sdlh*	env)
{
	SDL_DestroyTexture(env->texture);
	SDL_DestroyRenderer(env->renderer);
	SDL_DestroyWindow(env->window);
	SDL_FreeSurface(env->surface);
	SDL_Quit();
}

