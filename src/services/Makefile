FLAGS = -g
CC = g++

all: stest

stest: ServiceTest.o test/TestService.o config/ConfigService.o metaserver/MetaserverService.o
	g++ ServiceTest.o test/TestService.o config/ConfigService.o metaserver/MetaserverService.o -leris -lsigc -o stest		
	
ServiceTest.o: ServiceTest.cpp
	g++ -c ServiceTest.cpp -I../../../../libs/eris/src

clean:
	rm -R *.o
	rm stest

