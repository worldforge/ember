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
    glColor3f(color.getR()/255.0f,
              color.getG()/255.0f,
              color.getB()/255.0f);
    glVertex3f(x, y, -0.9f);
    glEnd();
    restoreMatrices();
}


void dime::OGLDrawDevice::drawLine(int x1, int y1, int x2, int y2, dime::Color color)
{
    saveMatrices();
    glBegin(GL_LINES);
    glColor3f(color.getR()/255.0f,
              color.getG()/255.0f,
              color.getB()/255.0f);
    glVertex3f(x1,y1, -0.9f);
    glVertex3f(x2,y2, -0.9f);
    glEnd();
    restoreMatrices();
}

void dime::OGLDrawDevice::blitSurface(SDL_Rect *srcRect, SDL_Rect *destRect, SDL_Surface *src)
{
  int destX = 0;
  int destY = 0;
  float destZ = -.9f;

  int srcX = 0;
  int srcY = 0;
  int srcW = 0;
  int srcH = 0;
  
  if(!srcRect)
  {
	  THROW("Must supply a non-null surface.");
  }
  if(!destRect)
  {
	  THROW("the destination Rect must not be null.");
  }
  if(!src)
  {
	  THROW("the srcRect must not be null.");
  }
  destX = destRect->x;
  destY = destRect->y;
 
 
  srcX =  srcRect->x;
  // Some error checking to make sure we don't run off the surface in any sort of direction.
  if(srcX < 0)
  {
	  THROW("the source Rectangles x coord cannot be less than 0.");
	  // Throw Exception
  }
  srcY = srcRect->y;
  if(srcY < 0)
  {
	  THROW("the source Rectnagle's y coord cannot be less than 0.");
	  // Throw Exception
  }
  srcW = srcRect->w;
  if(srcW < 0)
  {
	  THROW("The source Rectangle's width cannot be less than 0.");
  }
  if(srcW > src->w)
  {
	  // Throw Exception
	  THROW("The source Rectangle's width cannot go over the actual width of the surface.");
	  
  }
  srcH = srcRect->h;
  if(srcH < 0)
  {
	  THROW("The source Rectangle's height cannot be less than zero!");
  }
  if(srcH > src->h)
  {
	// Throw Exception
	THROW("The Source Rectangle's height cannot be greater than the surface's actual height");
  }
  
  GLenum theType = GL_UNSIGNED_BYTE;
  GLenum theFormat = GL_BGR;
  src = SDL_DisplayFormat(src);
  if(src->format->BitsPerPixel == 8)
  {
	THROW("TODO: The GL renderer doesn't handle 8 bit picture for right now");
  }
  else if(src->format->BitsPerPixel == 16)
  {
	LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::VERBOSE) << "16 bits"<< ENDM;
	theType = GL_UNSIGNED_SHORT_4_4_4_4;
	theFormat = GL_RGBA;
  }
  else if(src->format->BitsPerPixel == 24)
  {
	theType = GL_UNSIGNED_BYTE;
	theFormat = GL_RGB;
  }
  else if(src->format->BitsPerPixel == 32)
  {
	 
    	S_LOG_VERBOSE() << src->format->Rmask << " " << src->format->Gmask << " " << src->format->Bmask << " " << src->format->Amask << " bpp " << src->format->BitsPerPixel << " Bpp " << src->format->BytesPerPixel << ENDM;
	  theFormat = GL_BGRA;
	  theType = GL_UNSIGNED_INT_8_8_8_8_REV;
  }
  saveMatrices();
  GLint viewport[4];

  glGetIntegerv(GL_VIEWPORT, viewport);
  
  //Set the position where we start the drawing.
  glRasterPos3f(destX, viewport[3]-destY, destZ);
  
  //SDL_surface's start form the top left, while OpenGL does it from the bottom left.
  //This zoom flips them.
  glPixelZoom(1.0,-1.0);
  
  //Lock the surface because we are directly accessing the pixels.
  SDL_LockSurface(src);
  
  //Draw the appropriate amount of the pixels.
  glDrawPixels(srcW, srcH, theFormat, theType, src->pixels);
  
  //Unlock the surface.
  SDL_UnlockSurface(src);
  
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
    glVertex3f(destRect->x, destRect->y, -0.9f);
    glVertex3f(destRect->w+destRect->x, destRect->y, -0.9f);
    glVertex3f(destRect->w+destRect->x, destRect->h+destRect->y, -0.9f);
    glVertex3f(destRect->x, destRect->h+destRect->y, -0.9f);
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
    glVertex3f(destRect->x, destRect->y, -0.9f);
    glColor3f(rightTop.getR()/255.0,
              rightTop.getG()/255.0,
              rightTop.getB()/255.0);
    glVertex3f(destRect->w+destRect->x, destRect->y, -0.9f);
    glColor3f(rightBottom.getR()/255.0,
              rightBottom.getG()/255.0,
              rightBottom.getB()/255.0);
    glVertex3f(destRect->w+destRect->x, destRect->h+destRect->y, -0.9f);
    glColor3f(leftBottom.getR()/255.0,
              leftBottom.getG()/255.0,
              leftBottom.getB()/255.0);
    glVertex3f(destRect->x, destRect->h+destRect->y, -0.9f);
    
    glEnd();
    restoreMatrices();
}


void dime::OGLDrawDevice::saveMatrices()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0f, myScreenWidth, 0.0f, myScreenHeight,  -0.9f, 1.0f);
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
