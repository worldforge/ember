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
  //Ok, this function was annoying for me to write.  Not only is it not effecient,
  //it's hard to understand. I'm going to rewrite it tomorrow morning. -nikal 2002-07-10 12:30AM
  GLuint texture[1];
  //If our src bitmap is null, return.
  if(!src)
    {
      return;
    }
  //Avoid division by 0
  if(src->w <= 0 || src->h <= 0)
    {
      return;
    }
  GLfloat srcX, srcY, srcW, srcH;
  GLfloat destX, destY, destW, destH;
  if(!srcRect)
    {
      srcX = 0;
      srcY = 0;
      srcW = src->w;
      srcH = src->h;
    }
  else 
    {
      srcX = srcRect->x;
      srcY = srcRect->y;
      srcW = srcRect->w;
      srcH = srcRect->h;
    }
  //If the destination is null, then do we return? or continue?
  if(!destRect)
    {
      destX=0;
      destY=0;
    }
  else
    {
      destX = destRect->x;
      destY = destRect->y;
    }
  destW = srcW;
  destH = srcH;
  // we really need a way to cache GL textures. :)
  // Create The Texture 
  glGenTextures( 1, &texture[0] );
  
  // Typical Texture Generation Using Data From The Bitmap
  glBindTexture( GL_TEXTURE_2D, texture[0] );
  
  // Generate The Texture 
  glTexImage2D( GL_TEXTURE_2D, 0, 3, src->w,
		src->h, 0, GL_RGB,
		GL_UNSIGNED_BYTE, src->pixels );
  
  // Linear Filtering 
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  saveMatrices();
  glBindTexture( GL_TEXTURE_2D, texture[0] );
  glBegin(GL_QUADS);
  
  // Top Left Of The Texture and Quad 
  glTexCoord2f( srcX/src->w, srcY/src->h ); 
  glVertex3f( destX, destY, -1.0f );
  // Top Right Of The Texture and Quad 
  glTexCoord2f( srcW/src->w, srcY/src->h ); 
  glVertex3f(  destX+destW, destY, -1.0f );
  // Bottom Right Of The Texture and Quad 
  glTexCoord2f( srcW/src->w, srcH/src->h ); 
  glVertex3f(  destX+destW, destY+destW, -1.0f );
  // Top Left Of The Texture and Quad 
  glTexCoord2f( srcX/src->w, srcH/src->h ); 
  glVertex3f( destX,  destY+destW , -1.0f);

  glEnd();
  restoreMatrices();
}

void dime::OGLDrawDevice::update()
{
  // Probably should assume double-buffering
  SDL_GL_SwapBuffers( );
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

void dime::OGLDrawDevice::clearScreen()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void dime::OGLDrawDevice::init()
{
  /* Enable Texture Mapping ( NEW ) */
  glEnable( GL_TEXTURE_2D );

  /* Enable smooth shading */
  glShadeModel( GL_SMOOTH );

  /* Set the background black */
  glClearColor( 0.0f, 0.0f, 0.0f, 0.5f );

  /* Depth buffer setup */
  glClearDepth( 1.0f );

  /* Enables Depth Testing */
  glEnable( GL_DEPTH_TEST );

  /* The Type Of Depth Test To Do */
  glDepthFunc( GL_LEQUAL );

  /* Really Nice Perspective Calculations */
  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
  // TODO: please verify the correctness of this for me please thanks
  // Xmp

  // Height / width ration
  GLfloat ratio;
 
  // Protect against a divide by zero
  if ( myScreenHeight == 0 )
    myScreenHeight = 1;

  ratio = ( GLfloat )myScreenWidth / ( GLfloat )myScreenHeight;
    
  // Setup our viewport
  glViewport( 0, 0, ( GLsizei )myScreenWidth, ( GLsizei )myScreenHeight );

  // change to the projection matrix and set our viewing volume.
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );
    
  // Set our perspective
  gluPerspective( 45.0f, ratio, 0.1f, 100.0f );

  // Make sure we're chaning the model view and not the projection
  glMatrixMode( GL_MODELVIEW );

  // Reset The View
  glLoadIdentity( );
}
