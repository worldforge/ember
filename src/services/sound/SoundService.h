/*
    Copyright (C) 2002  Miguel Guzman Miranda (Aglanor)

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

#ifndef SOUNDSERVICE_H
#define SOUNDSERVICE_H


#include <framework/Service.h>
#include <framework/ConsoleObject.h>	// so this object is able to listen to console commands

/* here i should do the AL includes
#include <Eris/Connection.h>
#include <Eris/Player.h>
#include <Eris/Lobby.h>
#include <Eris/World.h>
*/

// #include <sigc++/object.h>

namespace dime {

/**
 * Dime Sound Service
 *
 * @author Miguel Guzman Miranda (Aglanor)
 *
 * @see dime::Service
 * @see dime::ConsoleObject
 */
class SoundService: public Service, public ConsoleObject
{
    //======================================================================
    // Private Variables
    //======================================================================
    private:




    //----------------------------------------------------------------------
    // Constructors & Destructor

  public:

    /** Creates a new SoundService using default values. */
    SoundService();


    /** Deletes a SoundService instance. */
    ~SoundService();

    //----------------------------------------------------------------------
    // Getters & Setters

    //----------------------------------------------------------------------
    // Methods

    Service::Status start();

    void stop(int code);



}; //SoundService

} // namespace dime

#endif
