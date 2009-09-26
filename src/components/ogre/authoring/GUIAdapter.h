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

namespace EmberOgre
{
namespace Authoring
{
/**
 * @brief GUI adapters wrapper.
 *
 * This class is used to wrap adapters that are instantiated with EmberOgre::Gui::Adapters::Atlas::AdapterFactory.
 * It allows to &ldquo;create&rdquo; adapter without window and later attach it to window.
 *
 * @author Alexey Torkhov <atorkhov@gmail.com>
 */
class GUIAdapter
{

	// This maps suggestion text to values
	typedef std::map<std::string, std::string> SuggestionsStore;

public:
	/**
	 * Constructor.
	 * @param type The type of new adapter. Should be known to AdaptersFactory.
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
	 * Detaches adapter from previously attached window.
	 */
	void detach();

	/**
	 * @brief Returns adapter value.
	 *
	 * This function returns adapter value.
	 * If allowRandom was set, it return random suggestion if value is equal to &ldquo;Random&rdquo;
	 */
	Atlas::Message::Element getValue();

	/**
	 * @brief Sets adapter title.
	 *
	 * This sets adapter title that could be reused later, when populating widget with adapters.
	 */
	void setTitle(const std::string& title);

	/**
	 * Gets adapter title.
	 */
	const std::string& getTitle() const;

	/**
	 * @brief Sets an optional tooltip for the adapter.
	 * @param tooltip The tooltip text.
	 */
	void setTooltip(const std::string& tooltip);

	/**
	 * @brief Gets the optional tool tip text for the adapter.
	 * @return The tool tip text.
	 */
	const std::string& getTooltip() const;

	/**
	 * @brief Sets a default value, to which the created gui adapter should be initialized when it's created.
	 * @param value The default value. This must be translatable to something that the adapter used can handle (i.e. if the adapter used is a numerical adapter, the default value must be parsable into to a number).
	 */
	void setDefaultValue(const std::string& value);

	/**
	 * @brief Gets the default value.
	 * when using this to set the initial value of an adapter, it should be parsed into a corresponding atlas element value.
	 * @return The default value, as a string.
	 */
	const std::string& getDefaultValue() const;

	/**
	 * Adds suggestion.
	 */
	void addSuggestion(const std::string& value, const std::string& text);

	/**
	 * @brief Is &ldquo;Random&rdquo; allowed.
	 *
	 * If set to true, adds &ldquo;Random&rdquo; suggestion.
	 */
	void setAllowRandom(bool val);

	/**
	 * Emitted when the value has been changed from a gui update.
	 */
	sigc::signal<void> EventValueChanged;

protected:
	/**
	 * Handles value change of underlying adapter and propagates it forward.
	 */
	void valueChanged();

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
	SuggestionsStore mSuggestions;

	/**
	 * If true, adds "Random" suggestion to the list.
	 */
	bool mAllowRandom;

	/**
	 * @brief An optional tooltip text for this adapter.
	 */
	std::string mTooltip;

	std::string mDefaultValue;
};

}
}

#endif
