
CC = gcc
CFLAGS = -Wall -Werror -Wconversion -std=c99
DEPS = basura/utiles.o
GAME_FILES = *.c */*.c
TEST= test.c
GAME_NAME = defendiendo

clear:
	clear

game: clear $(DEPS) $(GAME_FILES)
	$(CC) $(GAME_FILES) $(DEPS) $(CFLAGS) -o $(GAME_NAME)





