/*
 *  File:       KeyPressEvent.h
 *  Summary:    Defines the Base KeyPress Event.
 *  Written by:  Martin Pollard (aka Xmp).
 *               Copyright 2002.
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
#ifndef KEYPRESSEVENT_H
#define KEYPRESSEVENT_H

#include "Event.h"
#include <assert.h>

namespace dime {

  /**
   * Base class for all mouse button events.
   */
  class KeyPressEvent : public Event
  {
  public:
    
      enum State
          {
              PRESSED,
              RELEASED,
          };
      
    /**
     * Constructor
     *
     */
    KeyPressEvent(InputDevice *device, Widget *widget, char key, State state)
        : Event(device, widget), myState(state), myKey(key)
      {
          assert(device);
      }
      
      

    /**
     * Virtual Destructor
     *
     */
      virtual ~KeyPressEvent()
      {
          
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
      
    


  protected:
      char myKey;
      State myState;
  };
  

}//namespaces 


#endif

