/*
    Copyright (C) 2004  Erik Ogenvik
    
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

#ifndef __EmberPrerequisites_H__
#define __EmberPrerequisites_H__


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "OgreIncludes.h"

///include the Logging service, since we want logging available from most classes
///in most cases, use the S_LOG* defines
///such as:
///S_LOG_INFO("some info");
#include "framework/LoggingInstance.h"

#include "framework/Exception.h"


#endif
