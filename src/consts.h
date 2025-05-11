#ifndef CONSTS_H_
#define CONSTS_H_

#define NUM_ROWS 5
#define NUM_COLS 5
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500
#define SQUARE_WIDTH (SCREEN_WIDTH / NUM_ROWS)
#define SQUARE_HEIGHT (SCREEN_HEIGHT / NUM_COLS)
#define SQUARE_DARK_COLOR (SDL_Color){ .r = 0, .g = 0, .b = 0, .a = 0 }
#define SQUARE_LIGHT_COLOR (SDL_Color){ .r = 255, .g = 255, .b = 255, .a = 255 }

#endif // CONSTS_H_
