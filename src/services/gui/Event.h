/*
 *  File:       event.h
 *  Summary:    Base class for all Meadow Events.
 *  Written by:  Lakin Wecker (aka nikal).
 *               Copyright 2001 nikal.
 * 
 * This file is part of Dime.
 *
 * Meadow is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.     
 * 
 * Meadow is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.     
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 * 
 *
 * File COPYING should have a full copy of the GPL.
 *
 * 
 */

#ifndef EVENT_H
#define EVENT_H


#include <SDL/SDL.h>
#include <services/input/InputDevice.h>

namespace dime {
    
  class Widget;

  class Event {
  
  public:
    /**
     * Sets the time of event.
     * This is the time of this object's creation. 
     *
     * @author nikal
     *
     */
    Event(InputDevice *inputDevice, Widget *widget)
        : myWidget(widget), myInputDevice(inputDevice)
    {
      myTimeOfEvent = SDL_GetTicks();
    }

    //---------------------------------------------------------------------------------------------------
    // Getters

    /**
     * Returns the time this object was created which is the timeOfEvent.
     *
     * @return the time of this event.
     *
     */
    int getTimeOfEvent() 
      {
	return myTimeOfEvent;
      }
    
    Widget *getWidget()
      {
        return myWidget;
      }
    
    InputDevice *getInputDevice()
    {
        return myInputDevice;
    }
    
  protected:
    Widget *myWidget;
    InputDevice *myInputDevice;
    int myTimeOfEvent;
  };

}// namespace Meadow/Events


#endif
