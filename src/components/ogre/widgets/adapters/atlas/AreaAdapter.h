//
// C++ Interface: AreaAdapter
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
#ifndef EMBEROGRE_GUI_ADAPTERS_ATLASAREAADAPTER_H
#define EMBEROGRE_GUI_ADAPTERS_ATLASAREAADAPTER_H

#include "AdapterBase.h"
#include "components/ogre/manipulation/Polygon.h"
#include "components/ogre/manipulation/IPolygonPositionProvider.h"
#include "components/ogre/EmberEntity.h"

namespace EmberOgre {
class EmberEntity;
namespace Manipulation {
class Polygon;
}

namespace Gui {

namespace Adapters {

namespace Atlas {

class EntityAreaPolygonPositionProvider: public ::EmberOgre::Manipulation::IPolygonPositionProvider
{
public:

	EntityAreaPolygonPositionProvider(EmberEntity& entity);
	
	virtual float getHeightForPosition(const WFMath::Point<2>& localPosition);

protected:
	EmberEntity& mEntity;
};

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class AreaAdapter : public AdapterBase
{
public:
	AreaAdapter(const ::Atlas::Message::Element& element, CEGUI::PushButton* showButton, EmberEntity* entity);

	virtual ~AreaAdapter();
    
	/**
	Updates the gui with new values.
	*/
	virtual void updateGui(const ::Atlas::Message::Element& element);
    
    void showPolygon();
    
protected:

	CEGUI::PushButton* mShowButton;
	::EmberOgre::Manipulation::Polygon* mPolygon;
	EmberEntity* mEntity;
	
	EntityAreaPolygonPositionProvider* mPositionProvider;

	bool showButton_Clicked(const CEGUI::EventArgs& e);

	virtual void fillElementFromGui();
	virtual bool _hasChanges();
};

}

}

}

}

#endif
