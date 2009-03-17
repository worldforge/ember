//
// C++ Interface: TerrainModAdapter
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
#ifndef EMBEROGRE_GUI_ADAPTERS_ATLASTERRAINMODADAPTER_H
#define EMBEROGRE_GUI_ADAPTERS_ATLASTERRAINMODADAPTER_H
#include "AdapterBase.h"

#include <memory>

namespace EmberOgre {

class EmberEntity;

namespace Gui {

namespace Adapters {

namespace Atlas {

class PolygonAdapter;

template <class T, class WidgetT>
class ListBinder
{
public:
	typedef std::pair<const std::string&, T*> SelectedType;
	ListBinder(WidgetT* listbox);

	void addType(const std::string& key, const std::string& displayName, T type);
	
	sigc::signal<void, const std::string&, T&> EventSelected;
	
	void sync();
	
	SelectedType getCurrentSelected();
	
	T* select(const std::string& key);

protected:
	typedef std::map<std::string, std::pair<std::string, T> > TypeStore;
	typedef std::map<size_t, T*> TypeLookupStore;
	TypeStore mTypes;
	
	TypeLookupStore mListboxLookup;
	
	WidgetT* mListbox;

	bool listbox_SelectionChanged(const CEGUI::EventArgs& e);

};

class TerrainModBase {
public:

	const std::string& getType() const;

protected:
	TerrainModBase(const std::string& type);
	
	std::string mType;
};

class LevelTerrainMod : public TerrainModBase
{
public:
	LevelTerrainMod();
};

class AdjustTerrainMod : public TerrainModBase
{
public:
	AdjustTerrainMod();
};

class PositioningBase 
{
public:

	const std::string& getType() const;

protected:
	PositioningBase(const std::string& type);
	
	std::string mType;

};

class FixedPositioning : public PositioningBase
{
public:
	FixedPositioning();
};

class RelativePositioning : public PositioningBase
{
public:
	RelativePositioning();
};

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class TerrainModAdapter : public AdapterBase
{
public:
	
	TerrainModAdapter(const ::Atlas::Message::Element& element, CEGUI::PushButton* showButton, EmberEntity* entity, CEGUI::Combobox* posTypeCombobox, CEGUI::Combobox* modTypeCombobox, CEGUI::Editbox* heightTextbox);

	virtual ~TerrainModAdapter();
	
	/**
	 * @brief Updates the gui with new values.
	 */
	virtual void updateGui(const ::Atlas::Message::Element& element);

protected:
	
	/**
	 * @brief An optional entity to which the area belongs.
	 * Mainly used for height lookups, so that the polygon snaps to the ground.
	 */
	EmberEntity* mEntity;
	
	
	/**
	 * @brief The polygon adapter, which handles the actual polygon editing.
	 */
	std::auto_ptr<PolygonAdapter> mPolygonAdapter;
	
	CEGUI::Editbox* mHeightTextbox;
	
	ListBinder<TerrainModBase, CEGUI::Combobox> mTerrainModsBinder;
	ListBinder<PositioningBase, CEGUI::Combobox> mPositioningsBinder;
	
	
	bool heightTextbox_TextChanged(const CEGUI::EventArgs& e);
	
	virtual void fillElementFromGui();
	virtual bool _hasChanges();
};

}

}

}

}

#endif
