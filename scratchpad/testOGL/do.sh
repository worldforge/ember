#!/bin/bash

g++ -o test test.cpp ../../src/services/platform/SDLDrawDevice.o ../../src/services/platform/RectangleRenderer.o ../../src/services/platform/OGLDrawDevice.o -I../../src ../../src/services/image/libImageService.a -I/usr/local/include -I/usr/local/include/SDL -D_REENTRANT -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lSDL -lpthread -lSDL_image -lGL -lGLU
