/* 
   Copyright (C) 2002  Lakin Wecker

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "OGLDrawDevice.h"
#include <math.h>
#include <stdlib.h>


void dime::OGLDrawDevice::drawPixel(int x, int y, dime::Color color)
{
    saveMatrices();
    glBegin(GL_POINTS);
    glColor3f(color.getR()/255.0,
              color.getG()/255.0,
              color.getB()/255.0);
    glVertex3f(x, y, -1.0f);
    glEnd();
    restoreMatrices();
}


void dime::OGLDrawDevice::drawLine(int x1, int y1, int x2, int y2, dime::Color color)
{
    saveMatrices();
    glBegin(GL_LINES);
    glColor3f(color.getR()/255.0,
              color.getG()/255.0,
              color.getB()/255.0);
    glVertex3f(x1,y1, -1.0f);
    glVertex3f(x2,y2, -1.0f);
    glEnd();
    restoreMatrices();
}

void dime::OGLDrawDevice::blitSurface(SDL_Rect *srcRect, SDL_Rect *destRect, SDL_Surface *src)
{
    
}

void dime::OGLDrawDevice::update()
{

}

void dime::OGLDrawDevice::fillRect(SDL_Rect *destRect, dime::Color color)
{
    saveMatrices();
    glBegin(GL_QUADS);
    glColor3f(color.getR()/255.0,
              color.getG()/255.0,
              color.getB()/255.0);
    glVertex3f(destRect->x, destRect->y, -1.0f);
    glVertex3f(destRect->w, destRect->y, -1.0f);
    glVertex3f(destRect->w, destRect->h, -1.0f);
    glVertex3f(destRect->x, destRect->h, -1.0f);
    glEnd();
    restoreMatrices();
}

void dime::OGLDrawDevice::drawGradient(SDL_Rect *destRect, 
                                 dime::Color leftTop, 
                                 dime::Color rightTop, 
                                 dime::Color leftBottom, 
                                 dime::Color rightBottom)
{
    saveMatrices();
    glBegin(GL_QUADS);
    glColor3f(leftTop.getR()/255.0,
              leftTop.getG()/255.0,
              leftTop.getB()/255.0);
    glVertex3f(destRect->x, destRect->y, -1.0f);
    glColor3f(rightTop.getR()/255.0,
              rightTop.getG()/255.0,
              rightTop.getB()/255.0);
    glVertex3f(destRect->w, destRect->y, -1.0f);
    glColor3f(rightBottom.getR()/255.0,
              rightBottom.getG()/255.0,
              rightBottom.getB()/255.0);
    glVertex3f(destRect->w, destRect->h, -1.0f);
    glColor3f(leftBottom.getR()/255.0,
              leftBottom.getG()/255.0,
              leftBottom.getB()/255.0);
    glVertex3f(destRect->x, destRect->h, -1.0f);
    glEnd();
    restoreMatrices();
}


void dime::OGLDrawDevice::saveMatrices()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0f, myScreenWidth, myScreenHeight, 0.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}


void dime::OGLDrawDevice::restoreMatrices()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
