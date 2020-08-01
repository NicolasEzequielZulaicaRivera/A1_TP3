
CC = gcc
CFLAGS = -Wall -Werror -Wconversion -std=c99
DEPS = utiles/utiles.o
HEADERS = *.h */*.h
GAME_FILES = *.c */*.c
OTROS = caminos/_caminos.txt
TEST= test.c
GAME_NAME = defendiendo

clear:
	clear

game: clear $(DEPS) $(GAME_FILES)
	$(CC) $(GAME_FILES) $(DEPS) $(CFLAGS) -o $(GAME_NAME)

zip: clear $(DEPS) $(GAME_FILES) $(HEADERS) $(OTROS)
	zip TP3 $(DEPS) $(GAME_FILES) $(HEADERS) $(OTROS)





