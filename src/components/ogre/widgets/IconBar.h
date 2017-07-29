//
// C++ Interface: IconBar
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
#ifndef EMBEROGRE_WIDGETSICONBAR_H
#define EMBEROGRE_WIDGETSICONBAR_H
#include <string>
#include <vector>

namespace CEGUI
{
class Window;
class EventArgs;
}

namespace Ember {
namespace OgreView {


namespace Gui {
class IconBase;

/**
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class IconBar
{
public:
	IconBar(const std::string& name);
	virtual ~IconBar();
	
	void addIcon(IconBase* iconBase);
	void removeIcon(IconBase* iconBase);
	
	CEGUI::Window* getWindow();
	
	void setIconPadding(int iconPadding);

	float getAbsoluteHeight();
	float getAbsoluteWidth();

protected:
	typedef std::vector<IconBase*> IconBaseStore;
	
	CEGUI::Window* mWindow;
	
	IconBaseStore mIconBases;
	
	/**
	Repositions all icons as they are added and removed to the list.
	*/
	void repositionIcons();
	
	bool iconVisibilityChanged(const CEGUI::EventArgs& e);


	int mIconPadding;
};

}

}

}

#endif
