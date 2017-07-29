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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef RENDEREDCOLOURSTRINGTEXTCOMPONENT_H_
#define RENDEREDCOLOURSTRINGTEXTCOMPONENT_H_

#include <CEGUI/RenderedStringTextComponent.h>

namespace Ember
{
namespace Cegui
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

	RenderedColourStringTextComponent();

	RenderedColourStringTextComponent(const CEGUI::String& text, const CEGUI::String& font_name);

	void draw(const CEGUI::Window* ref_wnd, CEGUI::GeometryBuffer& buffer,
	              const CEGUI::Vector2f& position, const CEGUI::ColourRect* mod_colours,
	              const CEGUI::Rectf* clip_rect, const float vertical_space,
	              const float space_extra) const;

	CEGUI::RenderedStringTextComponent* clone() const;

	CEGUI::RenderedStringTextComponent* split(float split_point, bool first_component);
};

}
}
#endif /* RENDEREDCOLOURSTRINGTEXTCOMPONENT_H_ */
