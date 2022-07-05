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
void registerLua<Element>(sol::table& space) {
	auto element = space.new_usertype<Element>("Element", sol::no_constructor,
											   sol::base_classes, sol::bases<EventSet, PropertySet>());
	element["getParentElement"] = &Element::getParentElement;
	element["setArea"] = sol::overload(sol::resolve<const URect&>(&Element::setArea),
									   sol::resolve<const UVector2&, const USize&>(&Element::setArea),
									   sol::resolve<const UDim&, const UDim&, const UDim&, const UDim&>(&Element::setArea));
	element["getArea"] = &Element::getArea;
	element["setPosition"] = &Element::setPosition;
	element["setXPosition"] = &Element::setXPosition;
	element["setYPosition"] = &Element::setYPosition;
	element["getPosition"] = &Element::getPosition;
	element["getXPosition"] = &Element::getXPosition;
	element["getYPosition"] = &Element::getYPosition;
	element["setHorizontalAlignment"] = &Element::setHorizontalAlignment;
	element["getHorizontalAlignment"] = &Element::getHorizontalAlignment;
	element["setVerticalAlignment"] = &Element::setVerticalAlignment;
	element["getVerticalAlignment"] = &Element::getVerticalAlignment;
	element["setSize"] = &Element::setSize;
	element["getSize"] = &Element::getSize;
	element["setWidth"] = &Element::setWidth;
	element["getWidth"] = &Element::getWidth;
	element["setHeight"] = &Element::setHeight;
	element["getHeight"] = &Element::getHeight;
	element["setMinSize"] = &Element::setMinSize;
	element["getMinSize"] = &Element::getMinSize;
	element["setMaxSize"] = &Element::setMaxSize;
	element["getMaxSize"] = &Element::getMaxSize;
	element["setAspectMode"] = &Element::setAspectMode;
	element["getAspectMode"] = &Element::getAspectMode;
	element["setAspectRatio"] = &Element::setAspectRatio;
	element["getAspectRatio"] = &Element::getAspectRatio;
	element["setPixelAligned"] = &Element::setPixelAligned;
	element["isPixelAligned"] = &Element::isPixelAligned;
	element["getPixelPosition"] = &Element::getPixelPosition;
	element["getPixelSize"] = &Element::getPixelSize;
	element["calculatePixelSize"] = &Element::calculatePixelSize;
	element["getParentPixelSize"] = &Element::getParentPixelSize;
	element["getRotation"] = &Element::getRotation;
	element["setRotation"] = &Element::setRotation;
	element["addChild"] = &Element::addChild;
	element["removeChild"] = &Element::removeChild;
	element["getChildElementAtIdx"] = &Element::getChildElementAtIdx;
	element["getChildCount"] = &Element::getChildCount;
	element["isChild"] = &Element::isChild;
	element["isAncestor"] = &Element::isAncestor;
	element["setNonClient"] = &Element::setNonClient;
	element["isNonClient"] = &Element::isNonClient;
	element["getUnclippedOuterRect"] = &Element::getUnclippedOuterRect;
	element["getUnclippedInnerRect"] = &Element::getUnclippedInnerRect;
	element["getUnclippedRect"] = &Element::getUnclippedRect;
	element["getClientChildContentArea"] = &Element::getClientChildContentArea;
	element["getNonClientChildContentArea"] = &Element::getNonClientChildContentArea;
	element["getChildContentArea"] = &Element::getChildContentArea;
	element["notifyScreenAreaChanged"] = &Element::notifyScreenAreaChanged;
	element["getRootContainerSize"] = &Element::getRootContainerSize;


//String getProperty(const String& name) const
	auto namedElement = space.new_usertype<NamedElement>("NamedElement", sol::no_constructor,
														 sol::base_classes, sol::bases<Element, EventSet, PropertySet>()
	);
	namedElement["setName"] = &NamedElement::setName;
	namedElement["getName"] = string_getter(&NamedElement::getName);
	namedElement["getNamePath"] = string_getter(&NamedElement::getNamePath);
}