#!/bin/bash
CXX=g++
rm -Rf test
$CXX -o test test.cpp ../../src/services/platform/RectangleRenderer.o ../../src/services/platform/BitmapRenderer.o ../../src/services/platform/OGLDrawDevice.o -I../../src ../../src/services/image/libImageService.a -I/usr/local/include -I/usr/local/include/SDL -D_REENTRANT -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lSDL -lpthread -lSDL_image -L/usr/X11R6/lib -lGL -lGLU -g
