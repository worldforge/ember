FLAGS = -g
CC = g++

all: dime application.o 

dime: dime.cpp application.o
	$(CC) -o dime dime.cpp application.o `sdl-config --cflags --libs` $(FLAGS) 

application.o: application.cpp application.h
	$(CC) -c application.cpp `sdl-config --cflags` $(FLAGS)
