/*
 *  File:       MouseButtonevent.h
 *  Summary:    Defines the Base MouseButton Event.
 *  Written by:  Lakin Wecker (aka nikal).
 *               Copyright 2001.
 *
 *
 * This file is part of Dime.
 *
 * Dime is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.     
 * 
 * Dime is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.     
 *
 * You should have received a copy of the GNU General Public License
 * along with Dime; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 * 
 *
 * File COPYING should have a full copy of the GPL.
 *
 *
 */
#ifndef MOUSEBUTTONEVENT_H
#define MOUSEBUTTONEVENT_H

#include "Event.h"
#include <assert.h>

namespace dime {

  /**
   * Base class for all mouse button events.
   */
  class MouseButtonEvent : public Event
  {
  public:
         enum Button
          {
	      INVALIDBUTTON,
              LEFTBUTTON,
              MIDDLEBUTTON,
              RIGHTBUTTON
          };
      
      enum State
          {
              PRESSED,
              RELEASED,
              CLICKED,
              MULTICLICKED
          };
      
    /**
     * Constructor
     *
     */
    MouseButtonEvent(InputDevice *device, Widget *widget, Button button, State state)
        : Event(device, widget)
      {
          assert(device);
          myX = device->getAxisPosition(0);
          myY = device->getAxisPosition(1);
          myButton = button;
          myState = state;
      }
      
      

    /**
     * Virtual Destructor
     *
     */
      virtual ~MouseButtonEvent()
      {
          
      }
      
      /**
       * Returns the button for this event.
       *
       * @return Button for this event.
       */
      Button getButton()
      {
          return myButton;
      }
      
              
    /** 
     * Returns the type for this event.
     *
     * @return State for this event.
     */
      State getState()
      {
          return myState;
      }
      

    /**
     * Returns the position of the mouse in screen coordinates.
     *
     * @param x the int to store the x pos in.
     * @param y the int to store the y pos in.
     */
      void getPos(int &x, int &y)
      {
          x = myX;
          y = myY;
      }
      


  protected:
      Button myButton;
      int myX, myY;
      State myState;
  };
  

}//namespaces 


#endif

