FLAGS = -g
CC = g++

all: ConfigService

ConfigService: ConfigService.o
	g++ ConfigService.o -o ConfigService

ConfigService.o: ConfigService.cpp
	g++ -c ConfigService.cpp

clean:
	rm -R *.o
	rm ConfigService

