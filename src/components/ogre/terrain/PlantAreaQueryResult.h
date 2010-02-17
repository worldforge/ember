/*
 Copyright (C) 2010 erik

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

#ifndef PLANTAREAQUERYRESULT_H_
#define PLANTAREAQUERYRESULT_H_

#include <vector>

#include <OgreCommon.h>
#include <OgreMemoryAllocatorConfig.h>
#include <OgreVector3.h>
#include <OgreVector2.h>

namespace EmberOgre
{

namespace Terrain
{

template<typename> class Buffer;

class PlantAreaQuery;
class PlantAreaQueryResult
{
public:
	typedef Buffer<unsigned char> ShadowBuffer;
	/**
	A store of plant positions. We keep this in ogre space for performance reasons.
	*/
	typedef std::vector<Ogre::Vector3, Ogre::STLAllocator<Ogre::Vector3, Ogre::CategorisedAlignAllocPolicy<Ogre::MEMCATEGORY_GEOMETRY> > > PlantStore;

	PlantAreaQueryResult(const PlantAreaQuery& query);
	virtual ~PlantAreaQueryResult();

	PlantStore& getStore();
	const PlantStore& getStore() const;

	const PlantAreaQuery& getQuery() const;

	void getShadowColourAtWorldPosition(const Ogre::Vector2& position, Ogre::uint32& colour) const;
	void getShadowColourAtWorldPosition(const Ogre::Vector2& position, Ogre::ColourValue& colour) const;

	ShadowBuffer* getShadow() const;
	void setShadow(ShadowBuffer* shadow);

private:
	const PlantAreaQuery* mQuery;

	PlantStore mStore;

	ShadowBuffer* mShadow;
};

}

}

#endif /* PLANTAREAQUERYRESULT_H_ */
