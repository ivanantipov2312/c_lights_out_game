#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define NUM_COLS 5
#define NUM_ROWS 5

enum square_state {
	SQUARE_LIGHT,
	SQUARE_DARK,
};

struct square {
	enum square_state state;
	int x, y;
};

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
const int SQUARE_WIDTH = 500 / NUM_ROWS;
const int SQUARE_HEIGHT = 500 / NUM_COLS;

const SDL_Color SQUARE_DARK_COLOR = (SDL_Color){ .r = 0, .g = 0, .b = 0, .a = 255 };
const SDL_Color SQUARE_LIGHT_COLOR = (SDL_Color){ .r = 255, .g = 255, .b = 255, .a = 255 };

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int quit = 0;

struct square board[NUM_COLS][NUM_ROWS];
int lvl_number = 1;

void init_board(const char *board_file) {
	FILE *fp = fopen(board_file, "r");
	if (fp == NULL) {
		fprintf(stderr, "Failed to open board file!");
		exit(EXIT_FAILURE);
	}

	int c;
	int i = 0, j = 0;
	while ((c = getc(fp)) != EOF) {
		if (c == ' ')
			continue;

		if (c == '\n') {
			i++;
			j = 0;
		}

		if (c == '1' || c == '0') {
			enum square_state s = SQUARE_DARK;
			if (c == '1')
				s = SQUARE_LIGHT;

			board[i][j] = (struct square){ .state = s, .x = SQUARE_WIDTH * j, .y = SQUARE_HEIGHT * i };
			j++;
		
		}
	}
}

void handle_events_square(int x_idx, int y_idx)
{
	if (x_idx > NUM_ROWS - 1 || x_idx < 0 || y_idx > NUM_COLS - 1 || y_idx < 0)
		return;

	struct square tmp = board[y_idx][x_idx];
	if (tmp.state == SQUARE_DARK)
		tmp.state = SQUARE_LIGHT;
	else if (tmp.state == SQUARE_LIGHT)
		tmp.state = SQUARE_DARK;

	board[y_idx][x_idx].state = tmp.state;
}

void draw_square(struct square *s)
{
	SDL_FRect draw_rect = { .x = (float)s->x, .y = (float)s->y, .w = (float)SQUARE_WIDTH, .h = (float)SQUARE_HEIGHT };
	SDL_Color fill_color = s->state == SQUARE_LIGHT ? SQUARE_LIGHT_COLOR : SQUARE_DARK_COLOR;
	SDL_SetRenderDrawColor(renderer, fill_color.r, fill_color.g, fill_color.b, fill_color.a);
	SDL_RenderFillRect(renderer, &draw_rect);
}

void handle_events_board(SDL_Event *event)
{
	if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		int press_x = event->button.x;
		int press_y = event->button.y;

		int x_idx = (int)(press_x / SQUARE_WIDTH);
		int y_idx = (int)(press_y / SQUARE_HEIGHT);
		handle_events_square(x_idx, y_idx);

		int x_idx2 = x_idx + 1;
		int y_idx2 = y_idx;
		handle_events_square(x_idx2, y_idx2);

		int x_idx3 = x_idx - 1;
		int y_idx3 = y_idx;
		handle_events_square(x_idx3, y_idx3);

		int x_idx4 = x_idx;
		int y_idx4 = y_idx + 1;
		handle_events_square(x_idx4, y_idx4);

		int x_idx5 = x_idx;
		int y_idx5 = y_idx - 1;
		handle_events_square(x_idx5, y_idx5);
	}
}

void draw_board(void)
{
	for (int y = 0; y < NUM_COLS; y++) {
		for (int x = 0; x < NUM_ROWS; x++) {
			struct square tmp = board[y][x];
			draw_square(&tmp);
		}
	}
}

int check_board(void)
{
	for (int y = 0; y < NUM_COLS; y++) {
		for (int x = 0; x < NUM_ROWS; x++) {
			if (board[y][x].state == SQUARE_LIGHT)
				return 0;
		}
	}

	return 1;
}

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

void handle_events_sdl(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_EVENT_QUIT) {
			quit = 1;
			break;
		}
		handle_events_board(&event);
	}
}

void render_sdl(void)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	draw_board();
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
	init_board("assets/boards/lvl1.brd");

	while (!quit) {
		handle_events_sdl();
		render_sdl();

		if (check_board()) {
			char buf[32];
			sprintf(buf, "assets/boards/lvl%d.brd", lvl_number + 1);
			FILE *fp = fopen(buf, "r");
			if (fp != NULL) {
				lvl_number++;
				init_board(buf);
			} else {
				break;
			}
		}
	}
	quit_sdl();
	return 0;
}
