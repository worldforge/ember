//
// C++ Interface: GuiAdapter
//
// Description: 
//
//
// Author: Alexey Torkhov <atorkhov@gmail.com>, (C) 2008
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
#ifndef EMBEROGREGUIADAPTER_H
#define EMBEROGREGUIADAPTER_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include "components/ogre/widgets/adapters/atlas/AdapterBase.h"
#include <Atlas/Message/Element.h>
#include <CEGUIWindow.h>

namespace EmberOgre {

/**
 * Base class for GUI adapter.
 */
class GUIAdapter
{
public:
	/**
	 * Constructor.
	 */
	GUIAdapter(const std::string& type);

	/**
	 * Destructor.
	 */
	virtual ~GUIAdapter();

	/**
	 * Returns adapter type.
	 */
	const std::string& getType() const;

	/**
	 * Attaches adapter to window.
	 */
	void attach(CEGUI::Window* window);

	/**
	 * Detaches adapter from attached window.
	 */
	void detach();

	/**
	 * Returns adapter value.
	 */
	Atlas::Message::Element& getValue();

	/**
	 * Sets adapter title.
	 */
	void setTitle(const std::string& title);

	/**
	 * Gets adapter title.
	 */
	const std::string& getTitle() const;

	/**
	 * Adds suggestion.
	 */
	void addSuggestion(const std::string& text);

protected:
	/**
	 * Adapter type.
	 */
	std::string mType;

	/**
	 * Adapter container.
	 */
	::EmberOgre::Gui::Adapters::Atlas::AdapterBase* mAdapter;

	/**
	 * Adapter value.
	 */
	Atlas::Message::Element mElement;
	
	/**
	 * Adapter title.
	 */
	std::string mTitle;

	/**
	 * List of suggestions.
	 */
	std::list<std::string> mSuggestions;
};

}

#endif
