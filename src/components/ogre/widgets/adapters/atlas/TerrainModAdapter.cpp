//
// C++ Implementation: TerrainModAdapter
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "TerrainModAdapter.h"
#include "PolygonAdapter.h"

#include "../../ColouredListItem.h"

namespace EmberOgre {

namespace Gui {

namespace Adapters {

namespace Atlas {


template <typename T, typename WidgetT>
ListBinder<T, WidgetT>::ListBinder(WidgetT* listbox)
: mListbox(listbox)
{
// 	if (listbox) {
// 		addGuiEventConnection(textWindow->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&ListBinder<T, WidgetT>::listbox_SelectionChanged, this))); 
// 	}
}


template <typename T, typename WidgetT>
void ListBinder<T, WidgetT>::sync()
{
	mListbox->resetList();
	mListboxLookup.clear();
	for (typename TypeStore::iterator I = mTypes.begin(); I != mTypes.end(); ++I) {
		CEGUI::ListboxItem* item = new ColouredListItem(I->second.first, mListbox->getItemCount());
		mListbox->addItem(item);
		mListboxLookup[item->getID()] = &(I->second.second);
	}
}

template <typename T, typename WidgetT>
void ListBinder<T, WidgetT>::addType(const std::string& key, const std::string& displayName, T type)
{
	mTypes.insert(typename TypeStore::value_type(key, std::pair<std::string, T>(displayName, type)));
}

template <typename T, typename WidgetT>
bool ListBinder<T, WidgetT>::listbox_SelectionChanged(const CEGUI::EventArgs& e)
{
	
	T* selectedType(getCurrentSelected());
	for (typename TypeStore::iterator I = mTypes.begin(); I != mTypes.end(); ++I) {
		if (&(I->second.second) == selectedType) {
			EventSelected.emit(I->second.first, selectedType);
		}
	}
	return true;
}

template <typename T, typename WidgetT>
std::pair<const std::string&, T*> ListBinder<T, WidgetT>::getCurrentSelected()
{
	CEGUI::ListboxItem* item = mListbox->getSelectedItem ();
	if (item) {
		size_t index = item->getID();
		typename TypeLookupStore::iterator I = mListboxLookup.find(index);
		if (I != mListboxLookup.end()) {
			for (typename TypeStore::iterator J = mTypes.begin(); J != mTypes.end(); ++J) {
				if (&(J->second.second) == I->second) {
					return std::pair<const std::string&, T*>(J->first, I->second);
				}
			}
		}
	}
	return std::pair<const std::string&, T*>("", 0);
}

template <typename T, typename WidgetT>
T* ListBinder<T, WidgetT>::select(const std::string& key)
{
	typename TypeStore::iterator I = mTypes.find(key);
	if (I != mTypes.end()) {
		for (typename TypeLookupStore::iterator J = mListboxLookup.begin(); J != mListboxLookup.end(); ++J) {
			if (&(I->second.second) == J->second) {
				CEGUI::ListboxItem* item = mListbox->getListboxItemFromIndex(J->first);
				if (item) {
					mListbox->clearAllSelections();
					mListbox->setItemSelectState(item, true);
				}
				return J->second;
			}
		}
	}
	return 0;
}


TerrainModBase::TerrainModBase(const std::string& type)
: mType(type)
{
}

const std::string& TerrainModBase::getType() const
{
	return mType;
}

LevelTerrainMod::LevelTerrainMod()
: TerrainModBase("levelmod")
{
}

AdjustTerrainMod::AdjustTerrainMod()
: TerrainModBase("adjustmod")
{
}


PositioningBase::PositioningBase(const std::string& type)
: mType(type)
{
}

const std::string& PositioningBase::getType() const
{
	return mType;
}

FixedPositioning::FixedPositioning()
: PositioningBase("height")
{
}

RelativePositioning::RelativePositioning()
: PositioningBase("heightoffset")
{
}


TerrainModAdapter::TerrainModAdapter(const ::Atlas::Message::Element& element, CEGUI::PushButton* showButton, EmberEntity* entity, CEGUI::Combobox* posTypeCombobox, CEGUI::Combobox* modTypeCombobox, CEGUI::Editbox* heightTextbox)
: AdapterBase(element), mEntity(entity), mPolygonAdapter(0), mHeightTextbox(heightTextbox), mTerrainModsBinder(modTypeCombobox), mPositioningsBinder(posTypeCombobox)
{
	
	if (element.isMap()) {
		const ::Atlas::Message::MapType& areaData(element.asMap());
		::Atlas::Message::MapType::const_iterator I = areaData.find("shape");
		if (I != areaData.end()) {
			mPolygonAdapter = std::auto_ptr<PolygonAdapter>(new PolygonAdapter(I->second, showButton, entity));
		} else {
			mPolygonAdapter = std::auto_ptr<PolygonAdapter>(new PolygonAdapter(::Atlas::Message::Element(), showButton, entity));
		}
	} else {
		mPolygonAdapter = std::auto_ptr<PolygonAdapter>(new PolygonAdapter(::Atlas::Message::Element(), showButton, entity));
	}
	
	if (heightTextbox) {
		addGuiEventConnection(heightTextbox->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&TerrainModAdapter::heightTextbox_TextChanged, this))); 
	}
	mTerrainModsBinder.addType("levelmod", "Level", LevelTerrainMod());
	mTerrainModsBinder.addType("adjustmod", "Adjust", AdjustTerrainMod());
	
	mPositioningsBinder.addType("height", "Fixed", FixedPositioning());
	mPositioningsBinder.addType("heightoffset", "Relative", RelativePositioning());
	
	updateGui(element);
}


TerrainModAdapter::~TerrainModAdapter()
{
}

void TerrainModAdapter::updateGui(const ::Atlas::Message::Element& element)
{
	mPositioningsBinder.sync();
	mTerrainModsBinder.sync();
	if (element.isMap()) {
		const ::Atlas::Message::MapType& mapElement = element.asMap();
		::Atlas::Message::MapType::const_iterator I = mapElement.find("height");
		float height = 0;
		if (I != mapElement.end()) {
			mPositioningsBinder.select("height");
			if (I->second.isNum()) {
				height = I->second.asNum();
			}
		} else {
			I = mapElement.find("heightoffset");
			if (I != mapElement.end()) {
				mPositioningsBinder.select("heightoffset");
				if (I->second.isNum()) {
					height = I->second.asNum();
				}
			}
		}
		std::stringstream ss;
		ss << height;
		mHeightTextbox->setText(ss.str());
		
		I = mapElement.find("type");
		if (I != mapElement.end() && I->second.isString()) {
			mTerrainModsBinder.select(I->second.asString());
		}
	}
}

bool TerrainModAdapter::heightTextbox_TextChanged(const CEGUI::EventArgs& e)
{
	return true;
}

void TerrainModAdapter::fillElementFromGui()
{
	::Atlas::Message::MapType mapElement;
	::Atlas::Message::Element shapeElement = mPolygonAdapter->getChangedElement();
	if (shapeElement.isMap()) {
		shapeElement.asMap()["type"] = "polygon";
	}
	mapElement["shape"] = shapeElement;
	
	std::pair<const std::string&, PositioningBase*> positioning = mPositioningsBinder.getCurrentSelected();
	if (positioning.second) {
		mapElement[positioning.first] = ::Atlas::Message::Element(atof(mHeightTextbox->getText().c_str()));
	}
	std::pair<const std::string&, TerrainModBase*> modType = mTerrainModsBinder.getCurrentSelected();
	if (modType.second) {
		mapElement["type"] = modType.first;
	}
	
	mEditedElement = mapElement;
}

bool TerrainModAdapter::_hasChanges()
{
	return mOriginalElement != getChangedElement();
}
}

}

}

}
