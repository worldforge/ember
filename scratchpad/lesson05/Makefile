CC = gcc -Wall -ansi

all:
	$(CC) lesson05.c -o lesson05 -lGL -lGLU `sdl-config --cflags --libs` -L/usr/X11R6/lib

clean:
	@echo Cleaning up...
	@rm lesson05
	@echo Done.
