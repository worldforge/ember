/*
 Copyright (C) 2011 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef COLOUREDRENDEREDSTRINGPARSER_H_
#define COLOUREDRENDEREDSTRINGPARSER_H_
#include <CEGUIBasicRenderedStringParser.h>

namespace Ember
{
namespace OgreView
{
namespace Gui
{

/**
 * @brief Overrides the default behaviour of CEGUI::BasicRenderedStringParser to use colour modulation.
 * Since the CEGUI::RenderedStringTextComponent will use colour modulation and this will result in all colours being black if the modulation colour is black we need to use an instance of RenderedColourStringTextComponent whenever a colour different from the base colour is to be used.
 */
class ColouredRenderedStringParser: public CEGUI::BasicRenderedStringParser
{
protected:
	void appendRenderedText(CEGUI::RenderedString& rs, const CEGUI::String& text) const;

};

}
}
}
#endif /* COLOUREDRENDEREDSTRINGPARSER_H_ */
