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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGREGUIADAPTER_H
#define EMBEROGREGUIADAPTER_H

#include "components/ogre/EmberOgrePrerequisites.h"
#include "components/ogre/widgets/adapters/atlas/AdapterBase.h"
#include <Atlas/Message/Element.h>
#include <CEGUI/Window.h>

namespace Ember
{
namespace OgreView
{
namespace Authoring
{
/**
 * @brief GUI adapters wrapper.
 *
 * This class is used to wrap adapters that are instantiated with OgreView::Gui::Adapters::Atlas::AdapterFactory.
 * It allows to &ldquo;create&rdquo; adapter without window and later attach it to window.
 *
 * @author Alexey Torkhov <atorkhov@gmail.com>
 */
struct GUIAdapter
{
	/**
	 * Adapter type.
	 */
	std::string mType;


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
	std::map<std::string, std::string> mSuggestions;

	/**
	 * If true, allows randomizing by default.
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

}

#endif
