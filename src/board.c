#include <stdlib.h>
#include <stdio.h>
#include "board.h"

static void square_handle_events(struct board *b, int x_idx, int y_idx)
{
	if (x_idx > NUM_ROWS - 1 || x_idx < 0 || y_idx > NUM_COLS - 1 || y_idx < 0)
		return;

	struct square tmp = b->board[y_idx][x_idx];
	if (tmp.state == SQUARE_DARK)
		tmp.state = SQUARE_LIGHT;
	else if (tmp.state == SQUARE_LIGHT)
		tmp.state = SQUARE_DARK;

	b->board[y_idx][x_idx].state = tmp.state;
}

static void square_draw(SDL_Renderer *renderer, struct square *s)
{
	SDL_FRect draw_rect = { .x = (float)s->x, .y = (float)s->y, .w = (float)SQUARE_WIDTH, .h = (float)SQUARE_HEIGHT };
	SDL_Color fill_color = s->state == SQUARE_LIGHT ? SQUARE_LIGHT_COLOR : SQUARE_DARK_COLOR;
	SDL_SetRenderDrawColor(renderer, fill_color.r, fill_color.g, fill_color.b, fill_color.a);
	SDL_RenderFillRect(renderer, &draw_rect);
}

struct board *board_init(const char *board_file) {
	struct board *b = malloc(sizeof *b);

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

			b->board[i][j] = (struct square){ .state = s, .x = SQUARE_WIDTH * j, .y = SQUARE_HEIGHT * i };
			j++;
		
		}
	}

	return b;
}



void board_handle_events(struct board *b, SDL_Event *event)
{
	if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		int press_x = event->button.x;
		int press_y = event->button.y;

		int x_idx = (int)(press_x / SQUARE_WIDTH);
		int y_idx = (int)(press_y / SQUARE_HEIGHT);
		square_handle_events(b, x_idx, y_idx);

		int x_idx2 = x_idx + 1;
		int y_idx2 = y_idx;
		square_handle_events(b, x_idx2, y_idx2);

		int x_idx3 = x_idx - 1;
		int y_idx3 = y_idx;
		square_handle_events(b, x_idx3, y_idx3);

		int x_idx4 = x_idx;
		int y_idx4 = y_idx + 1;
		square_handle_events(b, x_idx4, y_idx4);

		int x_idx5 = x_idx;
		int y_idx5 = y_idx - 1;
		square_handle_events(b, x_idx5, y_idx5);
	}
}

void board_draw(SDL_Renderer *renderer, struct board *b)
{
	for (int y = 0; y < NUM_COLS; y++) {
		for (int x = 0; x < NUM_ROWS; x++) {
			struct square tmp = b->board[y][x];
			square_draw(renderer, &tmp);
		}
	}
}

bool board_is_completed(struct board *b)
{
	for (int y = 0; y < NUM_COLS; y++) {
		for (int x = 0; x < NUM_ROWS; x++) {
			if (b->board[y][x].state == SQUARE_LIGHT)
				return 0;
		}
	}

	return 1;
}
