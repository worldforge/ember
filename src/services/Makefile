FLAGS = -g
CC = g++

all: stest ServiceManager.o

stest: ServiceTest.o test/TestService.o ServiceManager.o
	g++ ServiceTest.o test/TestService.o ServiceManager.o -o stest		
	
ServiceTest.o: ServiceTest.cpp
	g++ -c ServiceTest.cpp 

ServiceManager.o: ServiceManager.cpp
	g++ -c ServiceManager.cpp

clean:
	rm -R *.o
	rm stest

