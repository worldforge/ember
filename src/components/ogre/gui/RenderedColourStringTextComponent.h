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

#ifndef RENDEREDCOLOURSTRINGTEXTCOMPONENT_H_
#define RENDEREDCOLOURSTRINGTEXTCOMPONENT_H_

#include <CEGUIRenderedStringTextComponent.h>

namespace Ember
{
namespace OgreView
{
namespace Gui
{

/**
 * @brief Overrides colour modulation of CEGUI::RenderedStringTextComponent.
 * The default implementation of CEGUI::RenderedStringTextComponent will always use the mod_colours to modulate the final colour.
 * Since we use black text colours this means that all our colours will end up black (i.e. it will multiply by zero, which results in zero).
 * This class overrides this behaviour to always disable the mod_colours.
 */
class RenderedColourStringTextComponent : public CEGUI::RenderedStringTextComponent
{
public:

	RenderedColourStringTextComponent(const CEGUI::String& text, const CEGUI::String& font_name);

	void draw(CEGUI::GeometryBuffer& buffer, const CEGUI::Vector2& position, const CEGUI::ColourRect* mod_colours, const CEGUI::Rect* clip_rect, const float vertical_space, const float space_extra) const;

	CEGUI::RenderedStringTextComponent* clone() const;
};

}
}
}
#endif /* RENDEREDCOLOURSTRINGTEXTCOMPONENT_H_ */
