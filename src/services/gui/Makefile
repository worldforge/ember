
test: ParaGuiApplication.o ParaGuiService.o test.cpp
	gcc -o test test.cpp ParaGuiApplication.o ParaGuiService.o -I/usr/include/SDL -I/usr/local/include/paragui -I../../ -lparagui `sdl-config --libs`

ParaGuiApplication.o: ParaGuiApplication.cpp ParaGuiApplication.h
	gcc -c ParaGuiApplication.cpp -I/usr/include/SDL -I/usr/local/include/paragui

ParaGuiService.o: ParaGuiService.cpp ParaGuiService.h
	gcc -c ParaGuiService.cpp -I/usr/local/include/SDL -I/usr/local/include/paragui -I../../
