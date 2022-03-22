//
// C++ Implementation: OgreLogObserver
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2006
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "OgreLogObserver.h"

#include "framework/LoggingInstance.h"

#include <OgreString.h>

using namespace Ogre;
namespace Ember {
namespace OgreView {

OgreLogObserver::OgreLogObserver() = default;

OgreLogObserver::~OgreLogObserver() = default;

void OgreLogObserver::messageLogged(const String& message, LogMessageLevel lml, bool maskDebug, const String& logName, bool& skipThisMessage) {
	if (!skipThisMessage) {
		static std::string ogre("(Ogre) ");
		//if there's a problem setting material name, log as verbose as these messages are quite common and not necessarily errors since the Model format in many cases overrides the material defined in the mesh
		if (Ogre::StringUtil::startsWith(message, "Can't assign material", false)) {
			lml = Ogre::LML_TRIVIAL;
		}
		switch (lml) {
			case Ogre::LML_TRIVIAL:
				LoggingInstance("Ogre", Log::VERBOSE) << ogre << message;
				break;
			case Ogre::LML_NORMAL:
				LoggingInstance("Ogre", Log::INFO) << ogre << message;
				break;
			case Ogre::LML_WARNING:
				LoggingInstance("Ogre", Log::WARNING) << ogre << message;
				break;
			case Ogre::LML_CRITICAL:
				LoggingInstance("Ogre", Log::FAILURE) << ogre << message;
				break;

		}
	}
}

}
}
