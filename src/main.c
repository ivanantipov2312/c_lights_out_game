#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "board.h"
#include "consts.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
bool quit = false;

int lvl_number = 1;

void die_sdl(const char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, SDL_GetError());
	exit(EXIT_FAILURE);
}

void init_sdl(void)
{
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO))
		die_sdl("SDL_Init");

	window = SDL_CreateWindow("Lights Out", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (window == NULL)
		die_sdl("SDL_CreateWindow");

	renderer = SDL_CreateRenderer(window, NULL);
	if (renderer == NULL)
		die_sdl("SDL_CreateRenderer");
}

void handle_events_sdl(struct board *b)
{
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_EVENT_QUIT) {
			quit = true;
			break;
		}
		board_handle_events(b, &event);
	}
}

void render_sdl(struct board *b)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	board_draw(renderer, b);
	SDL_RenderPresent(renderer);
}

void quit_sdl(void)
{
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
}

int main(void)
{
	init_sdl();
	struct board *b = board_init("assets/boards/lvl1.brd");

	while (!quit) {
		handle_events_sdl(b);
		render_sdl(b);

		if (board_is_completed(b)) {
			char buf[32];
			sprintf(buf, "assets/boards/lvl%d.brd", lvl_number + 1);
			FILE *fp = fopen(buf, "r");
			if (fp != NULL) {
				lvl_number++;
				b = board_init(buf);
			} else {
				break;
			}
		}
	}
	quit_sdl();
	return 0;
}
