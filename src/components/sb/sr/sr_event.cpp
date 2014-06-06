/*
 *  sr_event.cpp - part of Motion Engine and SmartBody-lib
 *  Copyright (C) 2008  University of Southern California
 *
 *  SmartBody-lib is free software: you can redistribute it and/or
 *  modify it under the terms of the Lesser GNU General Public License
 *  as published by the Free Software Foundation, version 3 of the
 *  license.
 *
 *  SmartBody-lib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Lesser GNU General Public License for more details.
 *
 *  You should have received a copy of the Lesser GNU General Public
 *  License along with SmarBody-lib.  If not, see:
 *      http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 *  CONTRIBUTORS:
 *      Marcelo Kallmann, USC (currently UC Merced)
 */
 
# include <sr/sr_event.h>

//# define SR_USE_TRACE1  
//# include <sr/sr_trace.h>

//===================================== SrEvent =================================

SrEvent::SrEvent ()
 {
   init ();
 }

void SrEvent::init ()
 {
   type = EventNone;
   key = 0;
   button = 0;
   button1 = button2 = button3 = 0;
   ctrl = shift = alt = key = 0;
   width = height = 0;
   pixel_size = 0.05f;
 }

void SrEvent::init_lmouse ()
 {
   type = EventNone;
   key = 0;
   button = 0;
   button1 = button2 = button3 = 0;
   ctrl = shift = alt = key = 0;
   lmouse = mouse;
   lmouse = mouse;
   mouse.x = mouse.y = 0;
   mouseCoord.x = mouseCoord.y = 0;
 }

const char *SrEvent::type_name () const
 {
   switch ( type )
    { case EventNone : return "none";
      case EventPush : return "push";
      case EventDrag : return "drag";
      case EventRelease : return "release";
      case EventKeyboard : return "keyboard";
    }
   return "undefined?!";
 }

SrOutput& operator<< ( SrOutput& out, const SrEvent& e )
 {
   out << e.type_name();

   if ( e.type==SrEvent::EventKeyboard ) 
    out << " [" << (e.key? e.key:' ') << ':' << (int)e.key << ']';

   out << " POS:" << e.mouse <<
          " BUTS:" << (int)e.button1<<(int)e.button2<<(int)e.button3<<
          " ACS:" << (int)e.alt<<(int)e.ctrl<<(int)e.shift;

   return out;
 }

