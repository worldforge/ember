FLAGS = -g
CC = g++

all: Dime Application.o 

Dime: Dime.cpp Application.o
	$(CC) -o Dime Dime.cpp Application.o `sdl-config --cflags --libs` $(FLAGS) 

Application.o: Application.cpp Application.h
	$(CC) -c Application.cpp `sdl-config --cflags` $(FLAGS)
