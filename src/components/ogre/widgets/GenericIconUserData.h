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
#ifndef EMBEROGRE_GUIGENERICICONUSERDATA_H
#define EMBEROGRE_GUIGENERICICONUSERDATA_H

namespace Ember
{
namespace OgreView
{
namespace Gui
{

/**
 * @brief A very lightweight class which can be used as a "user data" instance in CEGUI.
 * CEGUI allows for "user data" to be attached to windows. The user data can be anything, and this class is used as a means to bind a generic icon to a CEGUI::Window.
 * Whenever it's used it must be wrapped in boost::any to allow for safe casting.
 * @author Erik Ogenvik <erik@ogenvik.org>
 * @author Tiberiu Paunescu <tpa12@sfu.ca>
 */
template <class IconType>
class GenericIconUserData
{
public:
	/**
	 * @brief Ctor.
	 * @param entityIcon The entity icon to which to which this user data belongs.
	 */
	GenericIconUserData(IconType& mGenericIcon);

	/**
	 * @brief Gets the entity icon instance to which this user data instance belongs.
	 * @return The entity icon to which to which this user data belongs.
	 */
	IconType& getIcon() const;
private:
	/**
	 * @brief The entity icon to which to which this user data belongs.
	 */
	IconType& mGenericIcon;
};

template <class IconType>
GenericIconUserData<IconType>::GenericIconUserData(IconType& icon)
: mGenericIcon(icon)
{
}

template <class IconType>
IconType& GenericIconUserData<IconType>::getIcon() const
{
	return mGenericIcon;
}

}
}

}

#endif
