FLAGS = -g
CC = g++

all: stest

stest: ServiceTest.o test/TestService.o config/ConfigService.o metaserver/MetaserverService.o
	g++ ServiceTest.o test/TestService.o config/ConfigService.o metaserver/MetaserverService.o -I../ -leris -lsigc -o stest		
	
ServiceTest.o: ServiceTest.cpp
	g++ -c ServiceTest.cpp -I../

clean:
	rm -R *.o
	rm stest

