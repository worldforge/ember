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
void registerLua<EventSet>(sol::table& space) {
	auto eventSet = space.new_usertype<EventSet>("EventSet", sol::no_constructor);
	eventSet["addEvent"] = string_setter(&EventSet::addEvent);
	eventSet["fireEvent"] = [](EventSet* self, const char* name, EventArgs args) { self->fireEvent(name, args); };
	eventSet["removeEvent"] = string_setter(&EventSet::removeEvent);
	eventSet["removeAllEvents"] = &EventSet::removeAllEvents;
	eventSet["isEventPresent"] = string_setter(&EventSet::isEventPresent);
	eventSet["isMuted"] = &EventSet::isMuted;
	eventSet["setMutedState"] = &EventSet::setMutedState;
	eventSet["subscribeEvent"] = sol::overload([](EventSet* self, const char* event, const sol::function& callback) {
		auto functor = std::make_shared<LuaFunctor>(callback);
		return self->subscribeEvent(event, [=](const EventArgs& eventArgs) {
			return functor->handle(eventArgs);
		});
	}, [](EventSet* self, const char* event, const sol::function& callback, const sol::table& this_object) {
		auto functor = std::make_shared<LuaFunctorWithSelf>(callback, this_object);
		return self->subscribeEvent(event, [=](const EventArgs& eventArgs) {
			return functor->handle(eventArgs);
		});
	});

	auto connection = space.new_usertype<Event::Connection>("Connection");
	connection["disconnect"] = [](Event::Connection* self) { (*self)->disconnect(); };

	space.new_usertype<EventArgs>("EventArgs", sol::no_constructor,
								  "handled", sol::property(&EventArgs::handled)
	);

	space.new_usertype<MouseCursorEventArgs>("MouseCursorEventArgs", sol::no_constructor,
											 "mouseCursor", sol::property(&MouseCursorEventArgs::mouseCursor),
											 "image", sol::property(&MouseCursorEventArgs::image),
											 sol::base_classes, sol::bases<EventArgs>()
	);

	space.new_usertype<WindowEventArgs>("WindowEventArgs",
										sol::constructors<WindowEventArgs(Window*)>(),
										"window", sol::property(&WindowEventArgs::window),
										sol::base_classes, sol::bases<EventArgs>()
	);

	space.new_usertype<ActivationEventArgs>("ActivationEventArgs", sol::no_constructor,
											"otherWindow", sol::property(&ActivationEventArgs::otherWindow),
											sol::base_classes, sol::bases<WindowEventArgs, EventArgs>()
	);
	space.new_usertype<HeaderSequenceEventArgs>("HeaderSequenceEventArgs", sol::no_constructor,
												"oldIdx", sol::property(&HeaderSequenceEventArgs::d_oldIdx),
												"newIdx", sol::property(&HeaderSequenceEventArgs::d_newIdx),
												sol::base_classes, sol::bases<WindowEventArgs, EventArgs>()
	);

	space["LeftButton"] = MouseButton::LeftButton;
	space["RightButton"] = MouseButton::RightButton;
	space["MiddleButton"] = MouseButton::MiddleButton;
	space["X1Button"] = MouseButton::X1Button;
	space["X2Button"] = MouseButton::X2Button;
	space["MouseButtonCount"] = MouseButton::MouseButtonCount;
	space["NoButton"] = MouseButton::NoButton;

	auto mouseEventArgs = space.new_usertype<MouseEventArgs>("MouseEventArgs", sol::no_constructor,
															 sol::base_classes, sol::bases<WindowEventArgs, EventArgs>());
	mouseEventArgs["position"] = sol::property(&MouseEventArgs::position);
	mouseEventArgs["moveDelta"] = sol::property(&MouseEventArgs::moveDelta);
	mouseEventArgs["button"] = sol::property(&MouseEventArgs::button);
	mouseEventArgs["sysKeys"] = sol::property(&MouseEventArgs::sysKeys);
	mouseEventArgs["wheelChange"] = sol::property(&MouseEventArgs::wheelChange);

	auto keyEventArgs = space.new_usertype<KeyEventArgs>("KeyEventArgs", sol::no_constructor,
														 sol::base_classes, sol::bases<WindowEventArgs, EventArgs>()
	);
	keyEventArgs["codepoint"] = sol::property(&KeyEventArgs::codepoint);
	keyEventArgs["scancode"] = sol::property(&KeyEventArgs::scancode);
	keyEventArgs["sysKeys"] = sol::property(&KeyEventArgs::sysKeys);

	space.new_usertype<DragDropEventArgs>("DragDropEventArgs", sol::no_constructor,
										  "dragDropItem", sol::property(&DragDropEventArgs::dragDropItem),
										  sol::base_classes, sol::bases<WindowEventArgs, EventArgs>()
	);

	space.new_usertype<TreeEventArgs>("TreeEventArgs", sol::no_constructor,
									  "treeItem", sol::property(&TreeEventArgs::treeItem),
									  sol::base_classes, sol::bases<WindowEventArgs, EventArgs>()
	);

	space.new_usertype<RenderQueueEventArgs>("RenderQueueEventArgs", sol::no_constructor,
											 "queueID", sol::property(&RenderQueueEventArgs::queueID),
											 sol::base_classes, sol::bases<EventArgs>()
	);


	space["toMouseCursorEventArgs"] = [](EventArgs* self) { return dynamic_cast<MouseCursorEventArgs*>(self); };
	space["toWindowEventArgs"] = [](EventArgs* self) { return dynamic_cast<WindowEventArgs*>(self); };
	space["toActivationEventArgs"] = [](EventArgs* self) { return dynamic_cast<ActivationEventArgs*>(self); };
	space["toHeaderSequenceEventArgs"] = [](EventArgs* self) { return dynamic_cast<HeaderSequenceEventArgs*>(self); };
	space["toMouseEventArgs"] = [](EventArgs* self) { return dynamic_cast<MouseEventArgs*>(self); };
	space["toKeyEventArgs"] = [](EventArgs* self) { return dynamic_cast<KeyEventArgs*>(self); };
	space["toDragDropEventArgs"] = [](EventArgs* self) { return dynamic_cast<DragDropEventArgs*>(self); };
	space["toTreeEventArgs"] = [](EventArgs* self) { return dynamic_cast<TreeEventArgs*>(self); };
	space["toRenderQueueEventArgs"] = [](EventArgs* self) { return dynamic_cast<RenderQueueEventArgs*>(self); };
	
}