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

#include "BindingsRepresentations.h"
#include "components/lua/Connector.h"
#include "../../RepresentationBase.h"
#include "../../CheckboxRepresentation.h"
#include "../../ComboboxRepresentation.h"
#include "../../EditboxRepresentation.h"
#include "../../SliderRepresentation.h"
#include "../../SingleAdapterRepresentationBase.h"
#include "components/ogre/widgets/ListHolder.h"

using namespace Ember::OgreView::Gui::Representations;
using namespace Ember::OgreView::Gui;
using namespace Ember::Lua;

void registerBindingsRepresentations(sol::state_view& lua) {


	auto Ember = lua["Ember"].get_or_create<sol::table>();
	auto OgreView = Ember["OgreView"].get_or_create<sol::table>();
	auto Gui = OgreView["Gui"].get_or_create<sol::table>();
	auto Representations = Gui["Representations"].get_or_create<sol::table>();

	Representations.new_usertype<RepresentationBase<Atlas::Message::Element>>("RepresentationBase<Atlas::Message::Element>",
																			  "getGuiRoot", &RepresentationBase<Atlas::Message::Element>::getGuiRoot,
																			  "setEditedValue", &RepresentationBase<Atlas::Message::Element>::setEditedValue,
																			  "getEditedValue", &RepresentationBase<Atlas::Message::Element>::getEditedValue,
																			  "applyChanges", &RepresentationBase<Atlas::Message::Element>::applyChanges,
																			  "getEventValueChangedSignal", [](RepresentationBase<Atlas::Message::Element>* self) {
				return LuaConnector::create(self->getEventValueChangedSignal());
			},
																			  "getOriginalValue", &RepresentationBase<Atlas::Message::Element>::getOriginalValue,
																			  "hasChanges", &RepresentationBase<Atlas::Message::Element>::hasChanges,
																			  "isRemoved", &RepresentationBase<Atlas::Message::Element>::isRemoved,
																			  "addSuggestion", &RepresentationBase<Atlas::Message::Element>::addSuggestion
	);

	Representations.new_usertype<RepresentationBase<varconf::Variable>>("RepresentationBase<varconf::Variable>",
																		"getGuiRoot", &RepresentationBase<varconf::Variable>::getGuiRoot,
																		"setEditedValue", &RepresentationBase<varconf::Variable>::setEditedValue,
																		"getEditedValue", &RepresentationBase<varconf::Variable>::getEditedValue,
																		"applyChanges", &RepresentationBase<varconf::Variable>::applyChanges,
																		"getEventValueChangedSignal", [](RepresentationBase<varconf::Variable>* self) {
				return LuaConnector::create(self->getEventValueChangedSignal());
			},
																		"getOriginalValue", &RepresentationBase<varconf::Variable>::getOriginalValue,
																		"hasChanges", &RepresentationBase<varconf::Variable>::hasChanges,
																		"isRemoved", &RepresentationBase<varconf::Variable>::isRemoved,
																		"addSuggestion", &RepresentationBase<varconf::Variable>::addSuggestion
	);
	Representations.new_usertype<CheckboxRepresentation<Atlas::Message::Element>>("CheckboxRepresentation<Atlas::Message::Element>",
																				  sol::constructors<CheckboxRepresentation<Atlas::Message::Element>(const Atlas::Message::Element&)>(),
																				  sol::base_classes, sol::bases<RepresentationBase<Atlas::Message::Element>>()
	);
	Representations.new_usertype<CheckboxRepresentation<varconf::Variable>>("CheckboxRepresentation<varconf::Variable>",
																			sol::constructors<CheckboxRepresentation<varconf::Variable>(const varconf::Variable&)>(),
																			sol::base_classes, sol::bases<RepresentationBase<varconf::Variable>>()
	);

	Representations.new_usertype<ComboboxRepresentation<Atlas::Message::Element, std::string>>("ComboboxRepresentation<Atlas::Message::Element, std::string>",
																							   sol::constructors<ComboboxRepresentation<Atlas::Message::Element, std::string>(const Atlas::Message::Element&, bool)>(),
																							   sol::base_classes, sol::bases<RepresentationBase<Atlas::Message::Element>>()
	);
	Representations.new_usertype<ComboboxRepresentation<varconf::Variable, std::string>>("ComboboxRepresentation<varconf::Variable, std::string>",
																						 sol::constructors<ComboboxRepresentation<varconf::Variable, std::string>(const varconf::Variable&, bool)>(),
																						 sol::base_classes, sol::bases<RepresentationBase<varconf::Variable>>()
	);
	Representations.new_usertype<ComboboxRepresentation<varconf::Variable, int>>("ComboboxRepresentation<varconf::Variable, int>",
																				 sol::constructors<ComboboxRepresentation<varconf::Variable, int>(const varconf::Variable&, bool)>(),
																				 sol::base_classes, sol::bases<RepresentationBase<varconf::Variable>>()
	);
	Representations.new_usertype<EditboxRepresentation<Atlas::Message::Element, std::string>>("EditboxRepresentation<Atlas::Message::Element, std::string>",
																							  sol::constructors<EditboxRepresentation<Atlas::Message::Element, std::string>(const Atlas::Message::Element&)>(),
																							  sol::base_classes, sol::bases<RepresentationBase<Atlas::Message::Element>>()
	);
	Representations.new_usertype<EditboxRepresentation<varconf::Variable, std::string>>("EditboxRepresentation<varconf::Variable, std::string>",
																						sol::constructors<EditboxRepresentation<varconf::Variable, std::string>(const varconf::Variable&)>(),
																						sol::base_classes, sol::bases<RepresentationBase<varconf::Variable>>()
	);

	Representations.new_usertype<SliderRepresentation<Atlas::Message::Element>>("SliderRepresentation<Atlas::Message::Element>",
																				sol::constructors<SliderRepresentation<Atlas::Message::Element>(const Atlas::Message::Element&, float, bool)>(),
																				sol::base_classes, sol::bases<RepresentationBase<Atlas::Message::Element>>()
	);
	Representations.new_usertype<SliderRepresentation<varconf::Variable>>("SliderRepresentation<varconf::Variable>",
																		  sol::constructors<SliderRepresentation<varconf::Variable>(const varconf::Variable&, float, bool)>(),
																		  sol::base_classes, sol::bases<RepresentationBase<varconf::Variable>>()
	);
	Representations.new_usertype<SliderRepresentation<float>>("SliderRepresentation<float>",
															  sol::constructors<SliderRepresentation<float>(float, float, bool)>(),
															  sol::base_classes, sol::bases<RepresentationBase<float>>()
	);

}
