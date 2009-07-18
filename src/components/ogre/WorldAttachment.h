/*
 Copyright (C) 2009 erik

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


#ifndef WORLDATTACHMENT_H_
#define WORLDATTACHMENT_H_

#include "components/ogre/IEntityAttachment.h"

namespace Ogre {
class SceneNode;
}

namespace EmberOgre {

namespace Terrain
{
class TerrainGenerator;
}

class IGraphicalRepresentation;
class WorldEmberEntity;
class EmberEntity;


class WorldAttachment : public IEntityAttachment
{
public:
	WorldAttachment(WorldEmberEntity& worldEntity, Ogre::SceneNode& worldNode, Terrain::TerrainGenerator& terrainGenerator);
	virtual ~WorldAttachment();

	virtual IGraphicalRepresentation* getGraphicalRepresentation() const;

	virtual EmberEntity& getAttachedEntity() const;

	virtual EmberEntity* getParentEntity() const;

	virtual IEntityAttachment* attachEntity(EmberEntity& entity);

	virtual void updateScale();

	void getOffsetForContainedNode(const IEntityAttachment& attachment, const WFMath::Point<3>& localPosition, WFMath::Vector<3>& offset);

	virtual void setControllerDelegate(IAttachmentControlDelegate* controllerDelegate);

protected:

	WorldEmberEntity& mWorldEntity;
	Ogre::SceneNode& mWorldNode;
	Terrain::TerrainGenerator& mTerrainGenerator;

};

}
#endif /* WORLDATTACHMENT_H_ */
