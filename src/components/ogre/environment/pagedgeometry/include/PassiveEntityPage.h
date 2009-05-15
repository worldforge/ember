/*-------------------------------------------------------------------------------------
Copyright (c) 2008 Erik Hjortsberg

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
    1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------------*/

//PassiveEntityPage.h
//PassiveEntityPage is an extension to PagedGeometry which displays entities as entities (no optimization techniques).
//-------------------------------------------------------------------------------------

#ifndef __EntityPage_H__
#define __EntityPage_H__

#include <Ogre.h>
#include "PagedGeometry.h"

namespace PagedGeometry {

/**
\brief The PassiveEntityPage class renders entities as entities (no optimization).

*/
class PassiveEntityPage: public GeometryPage
{
public:
	typedef std::vector<std::pair<Ogre::SceneNode*, Ogre::Entity*> > EntityStore;

	void init(::PagedGeometry::PagedGeometry *geom, const Ogre::Any &data);
	PassiveEntityPage();
	~PassiveEntityPage();

	void addEntity(Ogre::Entity *ent, const Ogre::Vector3 &position, const Ogre::Quaternion &rotation, const Ogre::Vector3 &scale, const Ogre::ColourValue &color);
	void removeEntities();
	void setFade(bool enabled, Ogre::Real visibleDist = 0, Ogre::Real invisibleDist = 0);

	void setVisible(bool visible);

private:
	Ogre::SceneManager *sceneMgr;
	EntityStore mEntities;

	static unsigned long GUID;
	static inline Ogre::String getUniqueID(const Ogre::String &prefix)
	{
		return prefix + Ogre::StringConverter::toString(++GUID);
	}
};

}
#endif
