/*
  Copyright (C) 2002 Miguel Guzman Miranda (Aglanor)

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

#include "SoundService.h"
#include "services/logging/LoggingService.h"
#include "services/gui/Console.h"
#include "services/gui/Tokeniser.h"

/*
#include <list>
#include <algorithm>
#include <iostream>
*/

namespace dime
{


  /* ctor */
  SoundService::SoundService()
  {

  }
	
  /* dtor */
  SoundService::~SoundService()
  {

  }
	
  /* Method for starting this service 	*/
  Service::Status SoundService::start()
  {
    setStatus(Service::OK);
    setRunning( true );

    return Service::OK;

  }

  /* Interface method for stopping this service 	*/
  void SoundService::stop(int code)
  {
    setStatus(Service::OK);
    setRunning( false );
  }

} // namespace dime
