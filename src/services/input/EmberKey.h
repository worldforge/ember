/*
    Copyright (C) 2002 Tim Enderling

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

#ifndef DIMEKEY_H
#define DIMEKEY_H


#include <SDL/SDL_keysym.h>
#include <SDL/SDL_events.h>
#include <SDL/SDL.h>
namespace dime
{
    

/**
 * General class to hold both SDLKey and the unicode value for a keypress
 */
class DimeKey
{
private:
    //my SDLKey
    SDLKey myKey;
    //my unicode value
    Uint16 myUnicode;
public:
    DimeKey(SDLKey key, Uint16 unicode)
        : myKey(key), myUnicode(unicode)
    {
    }

    ~DimeKey()
    {
    }
    
    SDLKey getKey() const
    {
        return myKey;
    }
    
    Uint16 getUnicode() const
    {
        return myUnicode;
    }
    
    void setKey(SDLKey key)
    {
        myKey = key;
    }
    
    void setUnicode(Uint16 unicode)
    {
        myUnicode = unicode;
    }
    
    
};//end class DimeKey
 
}//end namespace dime
#endif
