FLAGS = -g
CC = g++

all: TestService

TestService: TestService.o
	g++ TestService.o -o TestService

TestService.o: TestService.cpp
	g++ -c TestService.cpp 

clean:
	rm -R *.o
	rm TestService

