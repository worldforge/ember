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
void registerLua<Window>(sol::table& space) {
	auto window = space.new_usertype<Window>("Window",
											 sol::base_classes, sol::bases<NamedElement, Element, EventSet, PropertySet>()
	);
	window["getType"] = string_getter(&Window::getType);
	window["isDestroyedByParent"] = &Window::isDestroyedByParent;
	window["isAlwaysOnTop"] = &Window::isAlwaysOnTop;
	window["isDisabled"] = &Window::isDisabled;
	window["isEffectiveDisabled"] = &Window::isEffectiveDisabled;
	window["isVisible"] = &Window::isVisible;
	window["isActive"] = &Window::isActive;
	window["isClippedByParent"] = &Window::isClippedByParent;
	window["getID"] = &Window::getID;
//							 window["isChild"]= &Window::isChild;
	window["isChildRecursive"] = &Window::isChildRecursive;
	window["getChildAtIdx"] = &Window::getChildAtIdx;
	window["getChild"] = sol::overload(string_setter(sol::resolve<Window*(const String&) const>(&Window::getChild)),
									   sol::resolve<Window*(uint) const>(&Window::getChild));
	window["getChildRecursive"] = sol::overload(string_setter(sol::resolve<Window*(const String&) const>(&Window::getChildRecursive)),
												sol::resolve<Window*(uint) const>(&Window::getChildRecursive));
	window["getActiveChild"] = sol::overload(sol::resolve<Window*()>(&Window::getActiveChild), sol::resolve<const Window*() const>(&Window::getActiveChild));
//							 window["isAncestor"]= &Window::isAncestor,
	window["getFont"] = [](Window* self) { return self->getFont(); };
	window["getText"] = string_getter(&Window::getText);
//	window["getText"] = [](const Window* self) { return std::string(self->getText().c_str()); };
	window["getTextVisual"] = string_getter(&Window::getTextVisual);
	window["inheritsAlpha"] = &Window::inheritsAlpha;
	window["getAlpha"] = &Window::getAlpha;
	window["getEffectiveAlpha"] = &Window::getEffectiveAlpha;
	window["getParent"] = &Window::getParent;
	window["getMouseCursor"] = &Window::getMouseCursor;
	window["getUserData"] = &Window::getUserData;
	window["restoresOldCapture"] = &Window::restoresOldCapture;
	window["isZOrderingEnabled"] = &Window::isZOrderingEnabled;
	window["wantsMultiClickEvents"] = &Window::wantsMultiClickEvents;
	window["isMouseAutoRepeatEnabled"] = &Window::isMouseAutoRepeatEnabled;
	window["getAutoRepeatDelay"] = &Window::getAutoRepeatDelay;
	window["getAutoRepeatRate"] = &Window::getAutoRepeatRate;
	window["distributesCapturedInputs"] = &Window::distributesCapturedInputs;
	window["isUsingDefaultTooltip"] = &Window::isUsingDefaultTooltip;
	window["getTooltip"] = &Window::getTooltip;
	window["getTooltipType"] = string_getter(&Window::getTooltipType);
	window["getTooltipText"] = string_getter(&Window::getTooltipText);
	window["getTooltipTextIncludingInheritance"] = string_getter(&Window::getTooltipTextIncludingInheritance);
	window["inheritsTooltipText"] = &Window::inheritsTooltipText;
	window["isRiseOnClickEnabled"] = &Window::isRiseOnClickEnabled;
	window["getRootWindow"] = sol::overload(sol::resolve<Window*()>(&Window::getRootWindow), sol::resolve<const Window*() const>(&Window::getRootWindow));
	window["setAlwaysOnTop"] = &Window::setAlwaysOnTop;
	window["setEnabled"] = &Window::setEnabled;
	window["setDisabled"] = &Window::setDisabled;
	window["enable"] = &Window::enable;
	window["disable"] = &Window::disable;
	window["setVisible"] = &Window::setVisible;
	window["show"] = &Window::show;
	window["hide"] = &Window::hide;
	window["activate"] = &Window::activate;
	window["deactivate"] = &Window::deactivate;
	window["setID"] = &Window::setID;
	window["setText"] = string_setter(&Window::setText);
	window["appendText"] = string_setter(&Window::appendText);
//							 window["destroyChild"]= &Window::destroyChild,
	window["addChild"] = &Window::addChild;
	window["moveToFront"] = &Window::moveToFront;
	window["moveToBack"] = &Window::moveToBack;
	window["moveInFront"] = &Window::moveInFront;
	window["moveBehind"] = &Window::moveBehind;
	window["getZIndex"] = &Window::getZIndex;
	window["isInFront"] = &Window::isInFront;
	window["isBehind"] = &Window::isBehind;
	window["captureInput"] = &Window::captureInput;
	window["releaseInput"] = &Window::releaseInput;
	window["setRestoreOldCapture"] = &Window::setRestoreOldCapture;
	window["setAlpha"] = &Window::setAlpha;
	window["setInheritsAlpha"] = &Window::setInheritsAlpha;
	window["invalidate"] = sol::overload(sol::resolve<void()>(&Window::invalidate), sol::resolve<void(bool)>(&Window::invalidate));
	window["setMouseCursor"] = sol::overload(sol::resolve<void(const Image*)>(&Window::setMouseCursor), [](Window* self, const char* name) { self->setMouseCursor(name); });
	//window["setUserData"] = &Window::setUserData;

	window["setZOrderingEnabled"] = &Window::setZOrderingEnabled;
	window["setWantsMultiClickEvents"] = &Window::setWantsMultiClickEvents;
	window["setMouseAutoRepeatEnabled"] = &Window::setMouseAutoRepeatEnabled;
	window["setAutoRepeatDelay"] = &Window::setAutoRepeatDelay;
	window["setAutoRepeatRate"] = &Window::setAutoRepeatRate;
	window["setDistributesCapturedInputs"] = &Window::setDistributesCapturedInputs;
	window["setTooltip"] = &Window::setTooltip;
	window["setTooltipType"] = string_setter(&Window::setTooltipType);
	window["setTooltipText"] = string_setter(&Window::setTooltipText);
	window["setInheritsTooltipText"] = &Window::setInheritsTooltipText;
	window["setRiseOnClickEnabled"] = &Window::setRiseOnClickEnabled;
	window["setModalState"] = &Window::setModalState;
	window["setUserString"] = [](Window* self, const char* name, const char* value) { self->setUserString(name, value); };
	window["setMousePassThroughEnabled"] = &Window::setMousePassThroughEnabled;
	window["notifyScreenAreaChanged"] = &Window::notifyScreenAreaChanged;
	window["clone"] = &Window::clone;


	auto windowManager = space.new_usertype<WindowManager>("WindowManager", sol::no_constructor);
	windowManager["getSingleton"] = &WindowManager::getSingleton;
	windowManager["createWindow"] = sol::overload([](WindowManager* self, const char* type) { return self->createWindow(type); },
												  [](WindowManager* self, const char* type, const char* name) { return self->createWindow(type, name); });
	windowManager["destroyWindow"] = &WindowManager::destroyWindow;
	windowManager["destroyAllWindows"] = &WindowManager::destroyAllWindows;
	windowManager["isAlive"] = &WindowManager::isAlive;
	// "loadLayoutFromContainer", &WindowManager::loadLayoutFromContainer,
	windowManager["loadLayoutFromFile"] = [](WindowManager* self, const char* filename) { return self->loadLayoutFromFile(filename); };
	windowManager["isDeadPoolEmpty"] = &WindowManager::isDeadPoolEmpty;
	windowManager["cleanDeadPool"] = &WindowManager::cleanDeadPool;
}