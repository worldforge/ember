//
// C++ Implementation: OgreInfo
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "OgreInfo.h"

#if defined(__WIN32__)
#elif defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif


namespace EmberOgre {

OgreInfo::OgreInfo()
{
}


OgreInfo::~OgreInfo()
{
}


bool OgreInfo::isIndirect() const
{
#ifdef WIN32
	///TODO: add checks for win32 too
	return false;
#else
	const GLubyte* pcRenderer = glGetString(GL_RENDERER);
	const std::string renderer((const char*)pcRenderer);
	
	return renderer.find("Indirect") != std::string::npos;
#endif
	
}


}
