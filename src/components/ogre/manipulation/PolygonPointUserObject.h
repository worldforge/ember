//
// C++ Interface: PolygonPointUserObject
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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
#ifndef EMBEROGRE_MANIPULATIONPOLYGONPOINTUSEROBJECT_H
#define EMBEROGRE_MANIPULATIONPOLYGONPOINTUSEROBJECT_H

#include <OgreUserDefinedObject.h>
#include <sigc++/signal.h>

namespace EmberOgre {

namespace Manipulation {

class PolygonPoint;

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class PolygonPointUserObject : public Ogre::UserDefinedObject
{
public: 
	/**
	The type of UserDefinedObject
	*/
	static const std::string s_TypeName;
	
	
	PolygonPointUserObject(PolygonPoint& point);
	
	virtual ~PolygonPointUserObject();
	
	/**
	 *    Overloaded method for getting the type name of this instance.
	 * @param  
	 * @return 
	 */
	virtual const Ogre::String & getTypeName (void) const;
	
	
	PolygonPoint& getPoint();
	

	
// 	void translate(Ogre::Real verticalMovement);
	
	/**
	Emitted when the position of the base point has been updated
	*/
	sigc::signal<void> EventUpdatedPosition;
	
	/**
	 *    Marks the entity as "moved"
	 */
	void markAsMoved();

	/**
	 *    Resets the marking of the entity to the normal material (instead of the "moved" marking)
	 */
	void resetMarking();
	
	
private:
	PolygonPoint& mPoint;

};

}

}

#endif
