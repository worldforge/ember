//
// C++ Interface: OgreEntityRenderer
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
#ifndef EMBEROGREOGREENTITYRENDERER_H
#define EMBEROGREOGREENTITYRENDERER_H

#include "MovableObjectRenderer.h"

namespace EmberOgre {

/**
Renders a single Ogre::Entity to a EntityCEGUITexture.

@author Erik Hjortsberg
*/
class OgreEntityRenderer : public MovableObjectRenderer
{
public:
    OgreEntityRenderer(CEGUI::Window* image);

    virtual ~OgreEntityRenderer();
    
    /**
     * Renders the submitted Entity.
     * @param modelName a mesh namel
     */
    void showEntity(const std::string& mesh);
    
    /**
     * Returns the current rendered Entity, or null if none is set.
     * @return 
     */
    Ogre::Entity* getEntity();

protected:

	Ogre::Entity* mEntity;

	virtual Ogre::MovableObject* getMovableObject();
	void setEntity(Ogre::Entity* entity);


};

}

#endif
