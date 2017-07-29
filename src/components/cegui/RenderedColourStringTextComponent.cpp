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

 This class contains code lifted from the CEGUI project, specifically CEGUIRenderedStringTextComponent.cpp,
 which has copyright of:
 Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
 and is licensed under this license:

 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.

 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "RenderedColourStringTextComponent.h"

#include <CEGUI/System.h>
#include <CEGUI/GUIContext.h>
#include <CEGUI/Exceptions.h>
#include <CEGUI/TextUtils.h>
#include <CEGUI/Font.h>

using namespace CEGUI;

namespace Ember
{
namespace Cegui
{

RenderedColourStringTextComponent::RenderedColourStringTextComponent() :
		RenderedStringTextComponent()
{
}

RenderedColourStringTextComponent::RenderedColourStringTextComponent(const String& text, const String& font_name) :
		RenderedStringTextComponent(text, font_name)
{
}

void RenderedColourStringTextComponent::draw(const CEGUI::Window* ref_wnd, CEGUI::GeometryBuffer& buffer, const CEGUI::Vector2f& position, const CEGUI::ColourRect* /*mod_colours*/, const CEGUI::Rectf* clip_rect, const float vertical_space, const float space_extra) const
{
	RenderedStringTextComponent::draw(ref_wnd, buffer, position, 0, clip_rect, vertical_space, space_extra);
}

RenderedStringTextComponent* RenderedColourStringTextComponent::clone() const
{
	return new RenderedColourStringTextComponent(*this);
}

RenderedStringTextComponent* RenderedColourStringTextComponent::split(float split_point, bool first_component)
{
	const Font* fnt = d_font ? d_font : System::getSingleton().getDefaultGUIContext().getDefaultFont();

	// This is checked, but should never fail, since if we had no font our
	// extent would be 0 and we would never cause a split to be needed here.
	if (!fnt)
		CEGUI_THROW(InvalidRequestException( "RenderedStringTextComponent::split: "
		"unable to split with no font set."));

	// create 'left' side of split and clone our basic configuration
	RenderedColourStringTextComponent* lhs = new RenderedColourStringTextComponent;
	lhs->d_padding = d_padding;
	lhs->d_verticalFormatting = d_verticalFormatting;
	lhs->d_font = d_font;
	lhs->d_colours = d_colours;

	// calculate the 'best' place to split the text
	size_t left_len = 0;
	float left_extent = 0.0f;

	while (left_len < d_text.length()) {
		size_t token_len = getNextTokenLength(d_text, left_len);
		// exit loop if no more valid tokens.
		if (token_len == 0)
			break;

		const float token_extent = fnt->getTextExtent(d_text.substr(left_len, token_len));

		// does the next token extend past the split point?
		if (left_extent + token_extent > split_point) {
			// if it was the first token, split the token itself
			if (first_component && left_len == 0)
				left_len = ceguimax(static_cast<size_t>(1), fnt->getCharAtPixel(d_text.substr(0, token_len), split_point));

			// left_len is now the character index at which to split the line
			break;
		}

		// add this token to the left side
		left_len += token_len;
		left_extent += token_extent;
	}

	// perform the split.
	lhs->d_text = d_text.substr(0, left_len);

	// here we're trimming leading delimiters from the substring range
	size_t rhs_start = d_text.find_first_not_of(TextUtils::DefaultWrapDelimiters, left_len);
	if (rhs_start == String::npos)
		rhs_start = left_len;

	d_text = d_text.substr(rhs_start);

	return lhs;
}
}
}
