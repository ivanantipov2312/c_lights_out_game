CC=gcc

SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin

CFLAGS=$(shell pkg-config --cflags sdl3) -I$(SRC_DIR) -std=c11
LDFLAGS=$(shell pkg-config --libs sdl3)

SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

$(BIN_DIR)/lights_out: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
