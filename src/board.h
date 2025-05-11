#ifndef BOARD_H_
#define BOARD_H_
#include <stdbool.h>
#include <SDL3/SDL.h>

#include "consts.h"

enum square_state {
	SQUARE_DARK,
	SQUARE_LIGHT,
};

struct square {
	enum square_state state;
	int x, y;
};

struct board {
	struct square board[NUM_ROWS][NUM_COLS];
};

struct board *board_init(const char *filepath);
void board_handle_events(struct board *b, SDL_Event *event);
void board_draw(SDL_Renderer *renderer, struct board *b);
bool board_is_completed(struct board *b);

#endif // BOARD_H_
