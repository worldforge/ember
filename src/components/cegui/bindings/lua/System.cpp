/*
 Copyright (C) 2022 Erik Ogenvik

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
#include "LuaFunctor.h"

using namespace CEGUI;
template <>
void registerLua<System>(sol::table& space) {
	auto system = space.new_usertype<System>("System", sol::no_constructor);
	system["getSingleton"] = &System::getSingleton;
	system["getDefaultGUIContext"] = &System::getDefaultGUIContext;

	auto guiContext = space.new_usertype<GUIContext>("GUIContext", sol::no_constructor);
	guiContext["getRootWindow"] = &GUIContext::getRootWindow;
	guiContext["getModalWindow"] = &GUIContext::getModalWindow;
	guiContext["getWindowContainingMouse"] = &GUIContext::getWindowContainingMouse;
	guiContext["getMouseCursor"] = sol::resolve<const MouseCursor&() const>(&GUIContext::getMouseCursor);
	guiContext["getMouseCursorConst"] = sol::resolve<const MouseCursor&() const>(&GUIContext::getMouseCursor);
	guiContext["getMouseCursorNonConst"] = sol::resolve<MouseCursor&()>(&GUIContext::getMouseCursor);

	auto udim = space.new_usertype<UDim>("UDim",
										 sol::constructors<UDim(), UDim(float, float)>(),
										 "scale", sol::property(&UDim::d_scale),
										 "offset", sol::property(&UDim::d_offset)
	);
	udim[sol::meta_method::addition] = [](UDim* lhs, UDim* rhs) { return *lhs + *rhs; };
	udim[sol::meta_method::subtraction] = [](UDim* lhs, UDim* rhs) { return *lhs - *rhs; };

	auto vector = space.new_usertype<UVector2>("UVector2",
											   sol::constructors<UVector2(), UVector2(UDim, UDim)>());
	vector["x"] = sol::property(&UVector2::d_x);
	vector["y"] = sol::property(&UVector2::d_y);


	auto usize = space.new_usertype<USize>("USize",
										   sol::constructors<UVector2(), UVector2(UDim, UDim)>()
	);
	usize["width"] = sol::property(&USize::d_width);
	usize["height"] = sol::property(&USize::d_height);

	auto urect = space.new_usertype<URect>("URect",
										   sol::constructors<URect(), URect(
												   const UVector2&, const UVector2&), URect(
												   const UDim&, const UDim&, const UDim&, const UDim&)>());
	urect["min"] = sol::property(&URect::d_min);
	urect["max"] = sol::property(&URect::d_max);
	urect["getPosition"] = &URect::getPosition;
	urect["getSize"] = &URect::getSize;
	urect["getWidth"] = &URect::getWidth;
	urect["getHeight"] = &URect::getHeight;
	urect["setPosition"] = &URect::setPosition;
	urect["setSize"] = &URect::setSize;
	urect["setWidth"] = &URect::setWidth;
	urect["setHeight"] = &URect::setHeight;
	urect["offset"] = &URect::offset;

	auto ubox = space.new_usertype<UBox>("UBox",
										 sol::constructors<UBox(), UBox(
												 const UDim&), UBox(
												 const UDim&, const UDim&, const UDim&, const UDim&), UBox(
												 const UBox&)>());
	ubox["top"] = sol::property(&UBox::d_top);
	ubox["left"] = sol::property(&UBox::d_left);
	ubox["bottom"] = sol::property(&UBox::d_bottom);
	ubox["right"] = sol::property(&UBox::d_right);

	auto vector2f = space.new_usertype<Vector2<float >>("Vector2f",
														sol::constructors<Vector2<float>(), Vector2<float>(float, float)>());
	vector2f["x"] = &Vector2<float>::d_x;
	vector2f["y"] = &Vector2<float>::d_y;

	auto vector3f = space.new_usertype<Vector3<float >>("Vector3f",
														sol::constructors<Vector2<float>(), Vector2<float>(float, float, float)>());
	vector3f["x"] = &Vector3<float>::d_x;
	vector3f["y"] = &Vector3<float>::d_y;
	vector3f["z"] = &Vector3<float>::d_z;

	auto sizef = space.new_usertype<Size<float >>("Sizef",
												  sol::constructors<Size<float>(), Size<float>(float, float)>());
	sizef["width"] = &Size<float>::d_width;
	sizef["height"] = &Size<float>::d_height;


	auto font = space.new_usertype<Font>("Font", sol::no_constructor,
										 sol::base_classes, sol::bases<PropertySet>());
	font["isCodepointAvailable"] = &Font::isCodepointAvailable;
	font["getTextExtent"] = [](Font* self, const char* text) { return self->getTextExtent(text); };
	font["getLineSpacing"] = [](Font* self) { return self->getLineSpacing(); };
	font["getFontHeight"] = [](Font* self) { return self->getFontHeight(); };
}