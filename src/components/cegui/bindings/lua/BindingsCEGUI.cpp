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

#include "BindingsCEGUI.h"
#include "Helper.h"
#include "framework/LoggingInstance.h"
#include "components/lua/Connector.h"

#include <CEGUI/CEGUI.h>

using namespace Ember::Cegui;
using namespace Ember::Lua;
using namespace CEGUI;

namespace {

template<typename T, typename... Args>
inline auto string_getter(String (T::* v)(const Args& ...) const) {
	return [=](T* self, const Args& ... args) -> std::string {
		return std::string(((*self).*v)(args...).c_str());
	};
}

template<typename T, typename... Args>
inline auto string_getter(const String& (T::* v)(const Args& ...) const) {
	return [=](T* self, const Args& ... args) -> std::string {
		return std::string(((*self).*v)(args...).c_str());
	};
}


template<typename T, typename ReturnT, typename... Args>
inline auto string_setter(ReturnT (T::* v)(const String&, const Args& ...)) {
	return [=](T* self, const char* string, const Args& ... args) -> ReturnT {
		return (((*self).*v)(string, args...));
	};
}

template<typename T, typename ReturnT, typename... Args>
inline auto string_setter(ReturnT (T::* v)(const String&, const Args& ...) const) {
	return [=](const T* self, const char* string, const Args& ... args) -> ReturnT {
		return (((*self).*v)(string, args...));
	};
}

template<typename T, typename... Args>
inline auto string_getter_setter(String (T::* v)(const String&, const Args& ...) const) {
	return [=](T* self, const char* string, const Args& ... args) -> std::string {
		return std::string(((*self).*v)(string, args...).c_str());
	};
}

template<typename T, typename... Args>
inline auto string_getter_setter(const String& (T::* v)(const String& string, const Args& ...) const) {
	return [=](T* self, const char* string, const Args& ... args) -> std::string {
		return std::string(((*self).*v)(string, args...).c_str());
	};
}


}

void registerBindingsCEGUI(sol::state_view& lua) {
	auto Ember = lua["Ember"].get_or_create<sol::table>();
	auto Cegui = Ember["Cegui"].get_or_create<sol::table>();

	auto CEGUI = lua["CEGUI"].get_or_create<sol::table>();

	auto eventSet = Cegui.new_usertype<EventSet>("EventSet");
	eventSet["addEvent"] = string_setter(&EventSet::addEvent);
	eventSet["fireEvent"] = [](EventSet* self, const char* name, EventArgs args) { self->fireEvent(name, args); };
	eventSet["removeEvent"] = string_setter(&EventSet::removeEvent);
	eventSet["removeAllEvents"] = &EventSet::removeAllEvents;
	eventSet["isEventPresent"] = string_setter(&EventSet::isEventPresent);
	eventSet["isMuted"] = &EventSet::isMuted;
	eventSet["setMutedState"] = &EventSet::setMutedState;
	eventSet["subscribeEvent"] = sol::overload([](EventSet* self, const char* event, sol::function callback) {
		return self->subscribeEvent(event, [=](const EventArgs& eventArgs) {
			if (callback) {
				auto result = callback(eventArgs);
				if (!result.valid()) {
					S_LOG_FAILURE(result.get<sol::error>());
					return false;
				} else {
					sol::object resultObject = result;
					//We don't require all Lua CEGUI event handlers to always return a boolean. If they don't we just return true here anyway.
					if (resultObject.is<bool>()) {
						return resultObject.as<bool>();
					} else {
						return true;
					}
				}
			} else {
				return true;
			}
		});
	}, [](EventSet* self, const char* event, sol::function callback, sol::table this_object) {
		return self->subscribeEvent(event, [=](const EventArgs& eventArgs) {
			if (callback) {
				auto result = callback(this_object, eventArgs);
				if (!result.valid()) {
					S_LOG_FAILURE(result.get<sol::error>());
					return false;
				} else {
					sol::object resultObject = result;
					//We don't require all Lua CEGUI event handlers to always return a boolean. If they don't we just return true here anyway.
					if (resultObject.is<bool>()) {
						return resultObject.as<bool>();
					} else {
						return true;
					}
				}
			} else {
				return true;
			}
		});
	});


	auto helper = Cegui.new_usertype<Helper>("Helper");
	helper["notifyScreenAreaChanged"] = &Helper::notifyScreenAreaChanged;
	helper["Window_getChildCount"] = &Helper::Window_getChildCount;
	helper["ImageManager_getSingleton"] = &Helper::ImageManager_getSingleton;
	helper["Window_getChildRecursive"] = &Helper::Window_getChildRecursive;
	helper["Window_removeChild"] = &Helper::Window_removeChild;
	helper["calculateRenderedCentredStringVerticalExtent"] = &Helper::calculateRenderedCentredStringVerticalExtent;

	auto propertySet = CEGUI.new_usertype<PropertySet>("PropertySet");

	propertySet["setProperty"] = [](Element* self, const char* name, const char* value) { self->setProperty(name, value); };
	//Can't use string_getter_setter because of templated getProperty, even with sol::resolve.
	propertySet["getProperty"] = [](const Element* self, const char* name) { return std::string(self->getProperty(name).c_str()); };
	propertySet["getPropertyDefault"] = [](const Element* self, const char* name) { return std::string(self->getPropertyDefault(name).c_str()); };
	propertySet["getPropertyHelp"] = [](const Element* self, const char* name) { return std::string(self->getPropertyHelp(name).c_str()); };
	propertySet["isPropertyPresent"] = [](const Element* self, const char* name) { return self->isPropertyPresent(name); };
	propertySet["isPropertyDefault"] = [](const Element* self, const char* name) { return self->isPropertyDefault(name); };

	auto element = CEGUI.new_usertype<Element>("Element",
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
	auto namedElement = CEGUI.new_usertype<NamedElement>("NamedElement",
														 sol::base_classes, sol::bases<Element, EventSet, PropertySet>()
	);
	namedElement["setName"] = &NamedElement::setName;
	namedElement["getName"] = string_getter(&NamedElement::getName);
	namedElement["getNamePath"] = string_getter(&NamedElement::getNamePath);


	auto window = CEGUI.new_usertype<Window>("Window",
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
	window["getFont"] = &Window::getFont;
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


	auto windowManager = CEGUI.new_usertype<WindowManager>("WindowManager");
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

	auto system = CEGUI.new_usertype<System>("System");
	system["getSingleton"] = &System::getSingleton;
	system["getDefaultGUIContext"] = &System::getDefaultGUIContext;

	auto guiContext = CEGUI.new_usertype<GUIContext>("GUIContext");
	guiContext["getRootWindow"] = &GUIContext::getRootWindow;
	guiContext["getModalWindow"] = &GUIContext::getModalWindow;
	guiContext["getWindowContainingMouse"] = &GUIContext::getWindowContainingMouse;
	guiContext["getMouseCursor"] = sol::resolve<const MouseCursor&() const>(&GUIContext::getMouseCursor);
	guiContext["getMouseCursorConst"] = sol::resolve<const MouseCursor&() const>(&GUIContext::getMouseCursor);
	guiContext["getMouseCursorNonConst"] = sol::resolve<MouseCursor&()>(&GUIContext::getMouseCursor);

	CEGUI.new_usertype<UDim>("UDim",
							 sol::constructors<UDim(), UDim(float, float)>(),
							 "scale", sol::var(&UDim::d_scale),
							 "offset", sol::var(&UDim::d_offset)
	);
	CEGUI.new_usertype<UVector2>("UVector2",
								 sol::constructors<UVector2(), UVector2(UDim, UDim)>(),
								 "x", sol::var(&UVector2::d_x),
								 "y", sol::var(&UVector2::d_y)
	);

	CEGUI.new_usertype<USize>("USize",
							  sol::constructors<UVector2(), UVector2(UDim, UDim)>(),
							  "width", sol::var(&USize::d_width),
							  "height", sol::var(&USize::d_height)
	);
	CEGUI.new_usertype<URect>("URect",
							  sol::constructors<URect(), URect(
									  const UVector2&, const UVector2&), URect(
									  const UDim&, const UDim&, const UDim&, const UDim&)>(),
							  "min", sol::var(&URect::d_min),
							  "max", sol::var(&URect::d_max),
							  "getPosition", &URect::getPosition,
							  "getSize", &URect::getSize,
							  "getWidth", &URect::getWidth,
							  "getHeight", &URect::getHeight,
							  "setPosition", &URect::setPosition,
							  "setSize", &URect::setSize,
							  "setWidth", &URect::setWidth,
							  "setHeight", &URect::setHeight,
							  "offset", &URect::offset
	);
	CEGUI.new_usertype<UBox>("UBox",
							 sol::constructors<UBox(), UBox(
									 const UDim&), UBox(
									 const UDim&, const UDim&, const UDim&, const UDim&), UBox(
									 const UBox&)>(),
							 "top", sol::var(&UBox::d_top),
							 "left", sol::var(&UBox::d_left),
							 "bottom", sol::var(&UBox::d_bottom),
							 "right", sol::var(&UBox::d_right)
	);

	CEGUI.new_usertype<EventArgs>("EventArgs",
								  "handled", sol::var(&EventArgs::handled)
	);

	CEGUI.new_usertype<MouseCursorEventArgs>("MouseCursorEventArgs",
											 "mouseCursor", sol::var(&MouseCursorEventArgs::mouseCursor),
											 "image", sol::var(&MouseCursorEventArgs::image),
											 sol::base_classes, sol::bases<EventArgs>()
	);

	CEGUI.new_usertype<WindowEventArgs>("WindowEventArgs",
										sol::constructors<WindowEventArgs(Window*)>(),
										"window", sol::var(&WindowEventArgs::window),
										sol::base_classes, sol::bases<EventArgs>()
	);

	CEGUI.new_usertype<ActivationEventArgs>("ActivationEventArgs",
											"otherWindow", sol::var(&ActivationEventArgs::otherWindow),
											sol::base_classes, sol::bases<WindowEventArgs, EventArgs>()
	);
	CEGUI.new_usertype<HeaderSequenceEventArgs>("HeaderSequenceEventArgs",
												"oldIdx", sol::var(&HeaderSequenceEventArgs::d_oldIdx),
												"newIdx", sol::var(&HeaderSequenceEventArgs::d_newIdx),
												sol::base_classes, sol::bases<WindowEventArgs, EventArgs>()
	);

	CEGUI["LeftButton"] = MouseButton::LeftButton;
	CEGUI["RightButton"] = MouseButton::RightButton;
	CEGUI["MiddleButton"] = MouseButton::MiddleButton;
	CEGUI["X1Button"] = MouseButton::X1Button;
	CEGUI["X2Button"] = MouseButton::X2Button;
	CEGUI["MouseButtonCount"] = MouseButton::MouseButtonCount;
	CEGUI["NoButton"] = MouseButton::NoButton;

	CEGUI.new_usertype<MouseEventArgs>("MouseEventArgs",
									   "position", sol::var(&MouseEventArgs::position),
									   "moveDelta", sol::var(&MouseEventArgs::moveDelta),
									   "button", sol::var(&MouseEventArgs::button),
									   "sysKeys", sol::var(&MouseEventArgs::sysKeys),
									   "wheelChange", sol::var(&MouseEventArgs::wheelChange),
									   sol::base_classes, sol::bases<WindowEventArgs, EventArgs>()
	);

	CEGUI.new_usertype<KeyEventArgs>("KeyEventArgs",
									 "codepoint", sol::var(&KeyEventArgs::codepoint),
									 "scancode", sol::var(&KeyEventArgs::scancode),
									 "sysKeys", sol::var(&KeyEventArgs::sysKeys),
									 sol::base_classes, sol::bases<WindowEventArgs, EventArgs>()
	);

	CEGUI.new_usertype<DragDropEventArgs>("DragDropEventArgs",
										  "dragDropItem", sol::var(&DragDropEventArgs::dragDropItem),
										  sol::base_classes, sol::bases<WindowEventArgs, EventArgs>()
	);

	CEGUI.new_usertype<TreeEventArgs>("TreeEventArgs",
									  "treeItem", sol::var(&TreeEventArgs::treeItem),
									  sol::base_classes, sol::bases<WindowEventArgs, EventArgs>()
	);

	CEGUI.new_usertype<RenderQueueEventArgs>("RenderQueueEventArgs",
											 "queueID", sol::var(&RenderQueueEventArgs::queueID),
											 sol::base_classes, sol::bases<EventArgs>()
	);


	CEGUI["toMouseCursorEventArgs"] = [](EventArgs* self) { return dynamic_cast<MouseCursorEventArgs*>(self); };
	CEGUI["toWindowEventArgs"] = [](EventArgs* self) { return dynamic_cast<WindowEventArgs*>(self); };
	CEGUI["toActivationEventArgs"] = [](EventArgs* self) { return dynamic_cast<ActivationEventArgs*>(self); };
	CEGUI["toHeaderSequenceEventArgs"] = [](EventArgs* self) { return dynamic_cast<HeaderSequenceEventArgs*>(self); };
	CEGUI["toMouseEventArgs"] = [](EventArgs* self) { return dynamic_cast<MouseEventArgs*>(self); };
	CEGUI["toKeyEventArgs"] = [](EventArgs* self) { return dynamic_cast<KeyEventArgs*>(self); };
	CEGUI["toDragDropEventArgs"] = [](EventArgs* self) { return dynamic_cast<DragDropEventArgs*>(self); };
	CEGUI["toTreeEventArgs"] = [](EventArgs* self) { return dynamic_cast<TreeEventArgs*>(self); };
	CEGUI["toRenderQueueEventArgs"] = [](EventArgs* self) { return dynamic_cast<RenderQueueEventArgs*>(self); };

	auto buttonBase = CEGUI.new_usertype<ButtonBase>("ButtonBase",
													 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	buttonBase["isHovering"] = &ButtonBase::isHovering;
	buttonBase["isPushed"] = &ButtonBase::isPushed;
	buttonBase["setPushedState"] = &ButtonBase::setPushedState;

	CEGUI.new_usertype<PushButton>("PushButton",
								   sol::base_classes, sol::bases<ButtonBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);

	auto toggleButton = CEGUI.new_usertype<ToggleButton>("ToggleButton",
														 sol::base_classes, sol::bases<ButtonBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	toggleButton["isSelected"] = &ToggleButton::isSelected;
	toggleButton["setSelected"] = &ToggleButton::setSelected;

	auto combobox = CEGUI.new_usertype<Combobox>("Combobox",
												 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	combobox["getSingleClickEnabled"] = &Combobox::getSingleClickEnabled;
	combobox["isDropDownListVisible"] = &Combobox::isDropDownListVisible;
	combobox["getEditbox"] = &Combobox::getEditbox;
	combobox["getPushButton"] = &Combobox::getPushButton;
	combobox["getDropList"] = &Combobox::getDropList;
	combobox["hasInputFocus"] = &Combobox::hasInputFocus;
	combobox["isReadOnly"] = &Combobox::isReadOnly;
	combobox["getTextMatchState"] = &Combobox::getTextMatchState;
	combobox["getValidationString"] = string_getter(&Combobox::getValidationString);
	combobox["getCaretIndex"] = &Combobox::getCaretIndex;
	combobox["getSelectionStartIndex"] = &Combobox::getSelectionStartIndex;
	combobox["getSelectionEndIndex"] = &Combobox::getSelectionEndIndex;
	combobox["getSelectionLength"] = &Combobox::getSelectionLength;
	combobox["getMaxTextLength"] = &Combobox::getMaxTextLength;
	combobox["activateEditbox"] = &Combobox::activateEditbox;
	combobox["getItemCount"] = &Combobox::getItemCount;
	combobox["getSelectedItem"] = &Combobox::getSelectedItem;
	combobox["getListboxItemFromIndex"] = &Combobox::getListboxItemFromIndex;
	combobox["getItemIndex"] = &Combobox::getItemIndex;
	combobox["isItemSelected"] = &Combobox::isItemSelected;
	combobox["findItemWithText"] = &Combobox::findItemWithText;
	combobox["isListboxItemInList"] = &Combobox::isListboxItemInList;
	combobox["resetList"] = &Combobox::resetList;
	combobox["addItem"] = &Combobox::addItem;
	combobox["insertItem"] = &Combobox::insertItem;
	combobox["removeItem"] = &Combobox::removeItem;
	combobox["clearAllSelections"] = &Combobox::clearAllSelections;
	combobox["setSortingEnabled"] = &Combobox::setSortingEnabled;
	combobox["setShowVertScrollbar"] = &Combobox::setShowVertScrollbar;
	combobox["setShowHorzScrollbar"] = &Combobox::setShowHorzScrollbar;
	combobox["setItemSelectState"] = sol::overload(sol::resolve<void(ListboxItem*, bool)>(&Combobox::setItemSelectState), sol::resolve<void(unsigned long, bool)>(&Combobox::setItemSelectState));
	combobox["handleUpdatedListItemData"] = &Combobox::handleUpdatedListItemData;
	combobox["isSortEnabled"] = &Combobox::isSortEnabled;
	combobox["isVertScrollbarAlwaysShown"] = &Combobox::isVertScrollbarAlwaysShown;
	combobox["isHorzScrollbarAlwaysShown"] = &Combobox::isHorzScrollbarAlwaysShown;
	combobox["showDropList"] = &Combobox::showDropList;
	combobox["hideDropList"] = &Combobox::hideDropList;
	combobox["setSingleClickEnabled"] = &Combobox::setSingleClickEnabled;
	combobox["setReadOnly"] = &Combobox::setReadOnly;
	combobox["setValidationString"] = string_setter(&Combobox::setValidationString);
	combobox["setCaretIndex"] = &Combobox::setCaretIndex;
	combobox["setSelection"] = &Combobox::setSelection;
	combobox["setMaxTextLength"] = &Combobox::setMaxTextLength;
	combobox["getAutoSizeListHeightToContent"] = &Combobox::getAutoSizeListHeightToContent;
	combobox["getAutoSizeListWidthToContent"] = &Combobox::getAutoSizeListWidthToContent;
	combobox["setAutoSizeListHeightToContent"] = &Combobox::setAutoSizeListHeightToContent;
	combobox["setAutoSizeListWidthToContent"] = &Combobox::setAutoSizeListWidthToContent;
	combobox["updateAutoSizedDropList"] = &Combobox::updateAutoSizedDropList;

	CEGUI.new_usertype<ComboDropList>("ComboDropList",
									  "setArmed", &ComboDropList::setArmed,
									  "isArmed", &ComboDropList::isArmed,
									  "setAutoArmEnabled", &ComboDropList::setAutoArmEnabled,
									  "isAutoArmEnabled", &ComboDropList::isAutoArmEnabled,
									  "resizeToContent", &ComboDropList::resizeToContent,
									  sol::base_classes, sol::bases<Listbox, Window, NamedElement, Element, PropertySet, EventSet>()
	);

	CEGUI.new_usertype<DragContainer>("DragContainer",
									  "isDraggingEnabled", &DragContainer::isDraggingEnabled,
									  "setDraggingEnabled", &DragContainer::setDraggingEnabled,
									  "isBeingDragged", &DragContainer::isBeingDragged,
									  "getPixelDragThreshold", &DragContainer::getPixelDragThreshold,
									  "setPixelDragThreshold", &DragContainer::setPixelDragThreshold,
									  "getDragAlpha", &DragContainer::getDragAlpha,
									  "setDragAlpha", &DragContainer::setDragAlpha,
									  "getDragCursorImage", &DragContainer::getDragCursorImage,
									  "setDragCursorImage", sol::overload([](DragContainer* self, const char* name) { self->setDragCursorImage(name); }, sol::resolve<const Image*>(&DragContainer::setDragCursorImage)),
									  "getCurrentDropTarget", &DragContainer::getCurrentDropTarget,
									  "isStickyModeEnabled", &DragContainer::isStickyModeEnabled,
									  "setStickyModeEnabled", &DragContainer::setStickyModeEnabled,
									  "pickUp", &DragContainer::pickUp,
									  "setFixedDragOffset", &DragContainer::setFixedDragOffset,
									  "getFixedDragOffset", &DragContainer::getFixedDragOffset,
									  "setUsingFixedDragOffset", &DragContainer::setUsingFixedDragOffset,
									  "isUsingFixedDragOffset", &DragContainer::isUsingFixedDragOffset,
									  sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);

	CEGUI.new_usertype<Editbox>("Editbox",
								"hasInputFocus", &Editbox::hasInputFocus,
								"isReadOnly", &Editbox::isReadOnly,
								"isTextMasked", &Editbox::isTextMasked,
								"getTextMatchState", &Editbox::getTextMatchState,
								"getValidationString", string_getter(&Editbox::getValidationString),
								"getCaretIndex", &Editbox::getCaretIndex,
								"getSelectionStartIndex", &Editbox::getSelectionStartIndex,
								"getSelectionEndIndex", &Editbox::getSelectionEndIndex,
								"getSelectionLength", &Editbox::getSelectionLength,
								"getMaskCodePoint", &Editbox::getMaskCodePoint,
								"getMaxTextLength", &Editbox::getMaxTextLength,
								"setReadOnly", &Editbox::setReadOnly,
								"setTextMasked", &Editbox::setTextMasked,
								"setValidationString", string_setter(&Editbox::setValidationString),
								"setCaretIndex", &Editbox::setCaretIndex,
								"setSelection", &Editbox::setSelection,
								"setMaskCodePoint", &Editbox::setMaskCodePoint,
								"setMaxTextLength", &Editbox::setMaxTextLength,
								sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);

	CEGUI.new_usertype<FrameWindow>("FrameWindow",
									"isSizingEnabled", &FrameWindow::isSizingEnabled,
									"isFrameEnabled", &FrameWindow::isFrameEnabled,
									"isTitleBarEnabled", &FrameWindow::isTitleBarEnabled,
									"isCloseButtonEnabled", &FrameWindow::isCloseButtonEnabled,
									"isRollupEnabled", &FrameWindow::isRollupEnabled,
									"isRolledup", &FrameWindow::isRolledup,
									"isDragMovingEnabled", &FrameWindow::isDragMovingEnabled,
									"setSizingEnabled", &FrameWindow::setSizingEnabled,
									"setFrameEnabled", &FrameWindow::setFrameEnabled,
									"setTitleBarEnabled", &FrameWindow::setTitleBarEnabled,
									"setCloseButtonEnabled", &FrameWindow::setCloseButtonEnabled,
									"setRollupEnabled", &FrameWindow::setRollupEnabled,
									"setDragMovingEnabled", &FrameWindow::setDragMovingEnabled,
									"getSizingBorderThickness", &FrameWindow::getSizingBorderThickness,
									"setSizingBorderThickness", &FrameWindow::setSizingBorderThickness,
									"toggleRollup", &FrameWindow::toggleRollup,
									"offsetPixelPosition", &FrameWindow::offsetPixelPosition,
									sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<GridLayoutContainer>("GridLayoutContainer",
//											"AP_Disabled", GridLayoutContainer::AP_Disabled,
//											"AP_LeftToRight", GridLayoutContainer::AP_LeftToRight,
//											"AP_TopToBottom", GridLayoutContainer::AP_TopToBottom,
											"setGridDimensions", &GridLayoutContainer::setGridDimensions,
											"getGridWidth", &GridLayoutContainer::getGridWidth,
											"getGridHeight", &GridLayoutContainer::getGridHeight,
											"setAutoPositioning", &GridLayoutContainer::setAutoPositioning,
											"getAutoPositioning", &GridLayoutContainer::getAutoPositioning,
											"setNextAutoPositioningIdx", &GridLayoutContainer::setNextAutoPositioningIdx,
											"getNextAutoPositioningIdx", &GridLayoutContainer::getNextAutoPositioningIdx,
											"autoPositioningSkipCells", &GridLayoutContainer::autoPositioningSkipCells,
											"addChildToPosition", &GridLayoutContainer::addChildToPosition,
											"getChildAtPosition", &GridLayoutContainer::getChildAtPosition,
											"removeChildFromPosition", &GridLayoutContainer::removeChildFromPosition,
											"swapChildPositions", sol::resolve<size_t, size_t, size_t, size_t>(&GridLayoutContainer::swapChildPositions),
											"swapChildren", sol::resolve<Window*, Window*>(&GridLayoutContainer::swapChildren),
											"moveChildToPosition", sol::resolve<Window*, size_t, size_t>(&GridLayoutContainer::moveChildToPosition),
											sol::base_classes, sol::bases<LayoutContainer, Window, NamedElement, Element, PropertySet, EventSet>()
	);

	CEGUI.new_usertype<GroupBox>("GroupBox",
								 "drawAroundWidget", [](GroupBox* self, const char* name) { return self->drawAroundWidget(name); },
								 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<HorizontalLayoutContainer>("HorizontalLayoutContainer",
												  sol::base_classes, sol::bases<SequentialLayoutContainer, LayoutContainer, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<ItemEntry>("ItemEntry",
								  "getItemPixelSize", &ItemEntry::getItemPixelSize,
								  "getOwnerList", &ItemEntry::getOwnerList,
								  "isSelected", &ItemEntry::isSelected,
								  "isSelectable", &ItemEntry::isSelectable,
								  "setSelected", &ItemEntry::setSelected,
								  "select", &ItemEntry::select,
								  "deselect", &ItemEntry::deselect,
								  "setSelectable", &ItemEntry::setSelectable,
								  sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);

	CEGUI.new_usertype<ItemListBase>("ItemListBase",
									 "Ascending", sol::var(ItemListBase::Ascending),
									 "Descending", sol::var(ItemListBase::Descending),
									 "UserSort", sol::var(ItemListBase::UserSort),
									 "getItemCount", &ItemListBase::getItemCount,
									 "getItemFromIndex", &ItemListBase::getItemFromIndex,
									 "getItemIndex", &ItemListBase::getItemIndex,
									 "findItemWithText", [](ItemListBase* self, const char* text, const ItemEntry* start_item) { return self->findItemWithText(text, start_item); },
									 "isItemInList", &ItemListBase::isItemInList,
									 "isAutoResizeEnabled", &ItemListBase::isAutoResizeEnabled,
									 "isSortEnabled", &ItemListBase::isSortEnabled,
									 "getSortMode", &ItemListBase::getSortMode,
									 "getItemRenderArea", &ItemListBase::getItemRenderArea,
									 "getContentPane", &ItemListBase::getContentPane,
									 "resetList", &ItemListBase::resetList,
									 "addItem", &ItemListBase::addItem,
									 "insertItem", &ItemListBase::insertItem,
									 "removeItem", &ItemListBase::removeItem,
									 "handleUpdatedItemData", &ItemListBase::handleUpdatedItemData,
									 "setAutoResizeEnabled", &ItemListBase::setAutoResizeEnabled,
									 "sizeToContent", &ItemListBase::sizeToContent,
									 "setSortEnabled", &ItemListBase::setSortEnabled,
									 "setSortMode", &ItemListBase::setSortMode,
									 "sortList", &ItemListBase::sortList,
									 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<ItemListbox>("ItemListbox",
									"getSelectedCount", &ItemListbox::getSelectedCount,
									"getLastSelectedItem", &ItemListbox::getLastSelectedItem,
									"getFirstSelectedItem", &ItemListbox::getFirstSelectedItem,
									"getNextSelectedItem", &ItemListbox::getNextSelectedItem,
									"getNextSelectedItemAfter", &ItemListbox::getNextSelectedItemAfter,
									"isMultiSelectEnabled", &ItemListbox::isMultiSelectEnabled,
									"isItemSelected", &ItemListbox::isItemSelected,
									"setMultiSelectEnabled", &ItemListbox::setMultiSelectEnabled,
									"clearAllSelections", &ItemListbox::clearAllSelections,
									"selectRange", &ItemListbox::selectRange,
									"selectAllItems", &ItemListbox::selectAllItems,
									sol::base_classes, sol::bases<ScrolledItemListBase, ItemListBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);

	CEGUI.new_usertype<LayoutContainer>("LayoutContainer",
										"markNeedsLayouting", &LayoutContainer::markNeedsLayouting,
										"needsLayouting", &LayoutContainer::needsLayouting,
										"layout", &LayoutContainer::layout,
										"layoutIfNecessary", &LayoutContainer::layoutIfNecessary,
										sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);

	CEGUI.new_usertype<Listbox>("Listbox",
								"getItemCount", &Listbox::getItemCount,
								"getSelectedCount", &Listbox::getSelectedCount,
								"getFirstSelectedItem", &Listbox::getFirstSelectedItem,
								"getNextSelected", &Listbox::getNextSelected,
								"getListboxItemFromIndex", &Listbox::getListboxItemFromIndex,
								"getItemIndex", &Listbox::getItemIndex,
								"findItemWithText", [](Listbox* self, const char* text, const ListboxItem* start_item) { return self->findItemWithText(text, start_item); },
								"isSortEnabled", &Listbox::isSortEnabled,
								"isMultiselectEnabled", &Listbox::isMultiselectEnabled,
								"isItemSelected", &Listbox::isItemSelected,
								"isListboxItemInList", &Listbox::isListboxItemInList,
								"isVertScrollbarAlwaysShown", &Listbox::isVertScrollbarAlwaysShown,
								"isHorzScrollbarAlwaysShown", &Listbox::isHorzScrollbarAlwaysShown,
								"resetList", &Listbox::resetList,
								"addItem", &Listbox::addItem,
								"insertItem", &Listbox::insertItem,
								"removeItem", &Listbox::removeItem,
								"clearAllSelections", &Listbox::clearAllSelections,
								"setSortingEnabled", &Listbox::setSortingEnabled,
								"setMultiselectEnabled", &Listbox::setMultiselectEnabled,
								"setShowVertScrollbar", &Listbox::setShowVertScrollbar,
								"setShowHorzScrollbar", &Listbox::setShowHorzScrollbar,
								"setItemSelectState", sol::overload(sol::resolve<ListboxItem*, bool>(&Listbox::setItemSelectState),
																	sol::resolve<size_t, bool>(&Listbox::setItemSelectState)),
								"handleUpdatedItemData", &Listbox::handleUpdatedItemData,
								"ensureItemIsVisible", sol::overload(sol::resolve<void(
																			 const ListboxItem*)>(&Listbox::ensureItemIsVisible),
																	 sol::resolve<void(size_t)>(&Listbox::ensureItemIsVisible)),
								"getListRenderArea", &Listbox::getListRenderArea,
								"getVertScrollbar", &Listbox::getVertScrollbar,
								"getHorzScrollbar", &Listbox::getHorzScrollbar,
								"getWidestItemWidth", &Listbox::getWidestItemWidth,
								"getTotalItemsHeight", &Listbox::getTotalItemsHeight,
								"getItemAtPoint", &Listbox::getItemAtPoint,
								sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);

	CEGUI.new_usertype<ListboxItem>("ListboxItem",
									"getText", string_getter(&ListboxItem::getText),
									"getTooltipText", string_getter(&ListboxItem::getTooltipText),
									"getID", &ListboxItem::getID,
									"getUserData", &ListboxItem::getUserData,
									"isSelected", &ListboxItem::isSelected,
									"isDisabled", &ListboxItem::isDisabled,
									"isAutoDeleted", &ListboxItem::isAutoDeleted,
									"getOwnerWindow", &ListboxItem::getOwnerWindow,
									"getSelectionColours", &ListboxItem::getSelectionColours,
									"getSelectionBrushImage", &ListboxItem::getSelectionBrushImage,
									"setText", string_setter(&ListboxItem::setText),
									"setTooltipText", string_setter(&ListboxItem::setTooltipText),
									"setID", &ListboxItem::setID,
									"setUserData", &ListboxItem::setUserData,
									"setSelected", &ListboxItem::setSelected,
									"setAutoDeleted", &ListboxItem::setAutoDeleted,
									"setSelectionColours", sol::overload(sol::resolve<void(
																				 const ColourRect&)>(&ListboxItem::setSelectionColours),
																		 sol::resolve<void(Colour, Colour, Colour, Colour)>(&ListboxItem::setSelectionColours),
																		 sol::resolve<void(Colour)>(&ListboxItem::setSelectionColours)),
									"setSelectionBrushImage", sol::overload(sol::resolve<void(
																					const Image*)>(&ListboxItem::setSelectionBrushImage),
																			[](ListboxItem* self, const char* name) { self->setSelectionBrushImage(name); }),
									"getPixelSize", &ListboxItem::getPixelSize
	);

	CEGUI.new_usertype<ListboxTextItem>("ListboxTextItem",
										"getFont", &ListboxTextItem::getFont,
										"getTextColours", &ListboxTextItem::getTextColours,
										"setFont", sol::overload(sol::resolve<void(Font*)>(&ListboxTextItem::setFont),
																 [](ListboxTextItem* self, const char* font) { self->setFont(font); }),
										"setTextColours", sol::overload(sol::resolve<void(
																				const ColourRect&)>(&ListboxTextItem::setTextColours),
																		sol::resolve<void(Colour, Colour, Colour, Colour)>(&ListboxTextItem::setTextColours),
																		sol::resolve<void(Colour)>(&ListboxTextItem::setTextColours)),
										"setTextParsingEnabled", &ListboxTextItem::setTextParsingEnabled,
										"isTextParsingEnabled", &ListboxTextItem::isTextParsingEnabled,
										sol::base_classes, sol::bases<ListboxItem>()
	);

	//ListHeader is unused
//	CEGUI.new_usertype<ListHeader>("ListHeader",
//								   "getColumnCount", &ListHeader::getColumnCount,
//								   "getSegmentFromColumn", &ListHeader::getSegmentFromColumn,
//								   "getSegmentFromID", &ListHeader::getSegmentFromID,
//								   "getSortSegment", &ListHeader::getSortSegment,
//								   "getColumnFromSegment", &ListHeader::getColumnFromSegment,
//								   "getColumnFromID", &ListHeader::getColumnFromID,
//								   "getSortColumn", &ListHeader::getSortColumn,
//								   "getColumnWithText", [](ListHeader* self, const char* text) { return self->getColumnWithText(text); },
//								   "getPixelOffsetToSegment", &ListHeader::getPixelOffsetToSegment,
//								   "getPixelOffsetToColumn", &ListHeader::getPixelOffsetToColumn,
//								   "getTotalSegmentsPixelExtent", &ListHeader::getTotalSegmentsPixelExtent,
//								   "getColumnWidth", &ListHeader::getColumnWidth,
//								   "getSortDirection", &ListHeader::getSortDirection,
//								   "isSortingEnabled", &ListHeader::isSortingEnabled,
//								   "isColumnSizingEnabled", &ListHeader::isColumnSizingEnabled,
//								   "isColumnDraggingEnabled", &ListHeader::isColumnDraggingEnabled,
//								   "getSegmentOffset", &ListHeader::getSegmentOffset,
//								   "setSortingEnabled", &ListHeader::setSortingEnabled,
//								   "setSortDirection", &ListHeader::setSortDirection,
//								   "setSortSegment", &ListHeader::setSortSegment,
//								   "setSortColumn", &ListHeader::setSortColumn,
//								   "setSortColumnFromID", &ListHeader::setSortColumnFromID,
//								   "setColumnSizingEnabled", &ListHeader::setColumnSizingEnabled,
//								   "setColumnDraggingEnabled", &ListHeader::setColumnDraggingEnabled,
//								   "addColumn",[](ListHeader* self, const char* text, unsigned int id, const UDim& width){self->addColumn(text, id, width);},
//								   "", &ListHeader::,
//								   "", &ListHeader::,
//								   "", &ListHeader::,
//								   "", &ListHeader::,
//								   "", &ListHeader::,
//								   "", &ListHeader::,
//								   "", &ListHeader::,
//								   "", &ListHeader::,
//
//								   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
//	);
	CEGUI.new_usertype<Menubar>("Menubar",
								sol::base_classes, sol::bases<MenuBase, ItemListBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<MenuBase>("MenuBase",
								 "getItemSpacing", &MenuBase::getItemSpacing,
								 "isMultiplePopupsAllowed", &MenuBase::isMultiplePopupsAllowed,
								 "getPopupMenuItem", &MenuBase::getPopupMenuItem,
								 "setItemSpacing", &MenuBase::setItemSpacing,
								 "changePopupMenuItem", &MenuBase::changePopupMenuItem,
								 "setAllowMultiplePopups", &MenuBase::setAllowMultiplePopups,
								 "getAutoCloseNestedPopups", &MenuBase::getAutoCloseNestedPopups,
								 "getPopupMenuItem", &MenuBase::getPopupMenuItem,
								 sol::base_classes, sol::bases<ItemListBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<MenuItem>("MenuItem",
								 "isHovering", &MenuItem::isHovering,
								 "isPushed", &MenuItem::isPushed,
								 "getPopupMenu", &MenuItem::getPopupMenu,
								 "setPopupMenu", &MenuItem::setPopupMenu,
								 "openPopupMenu", sol::overload(&MenuItem::openPopupMenu, [](MenuItem* self) { self->openPopupMenu(); }),
								 "closePopupMenu", sol::overload(&MenuItem::closePopupMenu, [](MenuItem* self) { self->closePopupMenu(); }),
								 "togglePopupMenu", &MenuItem::togglePopupMenu,
								 "hasAutoPopup", &MenuItem::hasAutoPopup,
								 "getAutoPopupTimeout", &MenuItem::getAutoPopupTimeout,
								 "setAutoPopupTimeout", &MenuItem::setAutoPopupTimeout,
								 "getPopupOffset", &MenuItem::getPopupOffset,
								 "setPopupOffset", &MenuItem::setPopupOffset,
								 sol::base_classes, sol::bases<ItemEntry, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<MCLGridRef>("MCLGridRef",
								   sol::constructors<MCLGridRef(unsigned int, unsigned int)>(),
								   "row", &MCLGridRef::row,
								   "column", &MCLGridRef::column
	);


	CEGUI.new_usertype<MultiColumnList>("MultiColumnList",
										"RowSingle", sol::var(MultiColumnList::RowSingle),
										"CellSingle", sol::var(MultiColumnList::CellSingle),
										"CellMultiple", sol::var(MultiColumnList::CellMultiple),
										"NominatedColumnSingle", sol::var(MultiColumnList::NominatedColumnSingle),
										"NominatedColumnMultiple", sol::var(MultiColumnList::NominatedColumnMultiple),
										"ColumnSingle", sol::var(MultiColumnList::ColumnSingle),
										"ColumnMultiple", sol::var(MultiColumnList::ColumnMultiple),
										"NominatedRowSingle", sol::var(MultiColumnList::NominatedRowSingle),
										"NominatedRowMultiple", sol::var(MultiColumnList::NominatedRowMultiple),
										"isUserSortControlEnabled", &MultiColumnList::isUserSortControlEnabled,
										"isUserColumnSizingEnabled", &MultiColumnList::isUserColumnSizingEnabled,
										"isUserColumnDraggingEnabled", &MultiColumnList::isUserColumnDraggingEnabled,
										"getColumnCount", &MultiColumnList::getColumnCount,
										"getRowCount", &MultiColumnList::getRowCount,
										"getSortColumn", &MultiColumnList::getSortColumn,
										"getColumnWithID", &MultiColumnList::getColumnWithID,
										"getColumnWithHeaderText", string_setter(&MultiColumnList::getColumnWithHeaderText),
										"getTotalColumnHeadersWidth", &MultiColumnList::getTotalColumnHeadersWidth,
										"getColumnHeaderWidth", &MultiColumnList::getColumnHeaderWidth,
										"getSortDirection", &MultiColumnList::getSortDirection,
										"getHeaderSegmentForColumn", &MultiColumnList::getHeaderSegmentForColumn,
										"getItemRowIndex", &MultiColumnList::getItemRowIndex,
										"getItemColumnIndex", &MultiColumnList::getItemColumnIndex,
										"getItemGridReference", &MultiColumnList::getItemGridReference,
										"getItemAtGridReference", &MultiColumnList::getItemAtGridReference,
										"isListboxItemInColumn", &MultiColumnList::isListboxItemInColumn,
										"isListboxItemInRow", &MultiColumnList::isListboxItemInRow,
										"isListboxItemInList", &MultiColumnList::isListboxItemInList,
//										"findColumnItemWithText", string_setter(&MultiColumnList::findColumnItemWithText),
//										"findRowItemWithText", string_setter(&MultiColumnList::findRowItemWithText),
//										"findListItemWithText", string_setter(&MultiColumnList::findListItemWithText),
										"getFirstSelectedItem", &MultiColumnList::getFirstSelectedItem,
										"getNextSelected", &MultiColumnList::getNextSelected,
										"getSelectedCount", &MultiColumnList::getSelectedCount,
										"isItemSelected", &MultiColumnList::isItemSelected,
										"getNominatedSelectionColumnID", &MultiColumnList::getNominatedSelectionColumnID,
										"getNominatedSelectionColumn", &MultiColumnList::getNominatedSelectionColumn,
										"getNominatedSelectionRow", &MultiColumnList::getNominatedSelectionRow,
										"getSelectionMode", &MultiColumnList::getSelectionMode,
										"isVertScrollbarAlwaysShown", &MultiColumnList::isVertScrollbarAlwaysShown,
										"isHorzScrollbarAlwaysShown", &MultiColumnList::isHorzScrollbarAlwaysShown,
										"getColumnID", &MultiColumnList::getColumnID,
										"getRowID", &MultiColumnList::getRowID,
										"getRowWithID", &MultiColumnList::getRowWithID,
										"getListRenderArea", &MultiColumnList::getListRenderArea,
										"getVertScrollbar", &MultiColumnList::getVertScrollbar,
										"getHorzScrollbar", &MultiColumnList::getHorzScrollbar,
										"getListHeader", &MultiColumnList::getListHeader,
										"getTotalRowsHeight", &MultiColumnList::getTotalRowsHeight,
										"getWidestColumnItemWidth", &MultiColumnList::getWidestColumnItemWidth,
										"getHighestRowItemHeight", &MultiColumnList::getHighestRowItemHeight,
										"resetList", &MultiColumnList::resetList,
										"addColumn", [](MultiColumnList* self, const char* text, unsigned int col_id, const UDim& width) { self->addColumn(text, col_id, width); },
										"insertColumn", [](MultiColumnList* self, const char* text, unsigned int col_id, const UDim& width, unsigned int position) { self->insertColumn(text, col_id, width, position); },
										"removeColumn", &MultiColumnList::removeColumn,
										"removeColumnWithID", &MultiColumnList::removeColumnWithID,
										"moveColumn", &MultiColumnList::moveColumn,
										"moveColumnWithID", &MultiColumnList::moveColumnWithID,
										"addRow", sol::overload([](MultiColumnList* self) { return self->addRow(); }, [](MultiColumnList* self, ListboxItem* item, unsigned int col_id) { return self->addRow(item, col_id); }),
										"insertRow", sol::overload([](MultiColumnList* self, unsigned int row_idx) { return self->insertRow(row_idx); }, [](MultiColumnList* self, ListboxItem* item, unsigned int col_id, unsigned int row_idx) { return self->insertRow(item, col_id, row_idx); }),
										"removeRow", &MultiColumnList::removeRow,
										"setItem", sol::overload(sol::resolve<ListboxItem*, const MCLGridRef&>(&MultiColumnList::setItem), sol::resolve<ListboxItem*, unsigned int, unsigned int>(&MultiColumnList::setItem)),
										"setSelectionMode", &MultiColumnList::setSelectionMode,
										"setNominatedSelectionColumnID", &MultiColumnList::setNominatedSelectionColumnID,
										"setNominatedSelectionColumn", &MultiColumnList::setNominatedSelectionColumn,
										"setNominatedSelectionRow", &MultiColumnList::setNominatedSelectionRow,
										"setSortDirection", &MultiColumnList::setSortDirection,
										"setSortColumn", &MultiColumnList::setSortColumn,
										"setSortColumnByID", &MultiColumnList::setSortColumnByID,
										"setShowVertScrollbar", &MultiColumnList::setShowVertScrollbar,
										"setShowHorzScrollbar", &MultiColumnList::setShowHorzScrollbar,
										"clearAllSelections", &MultiColumnList::clearAllSelections,
										"setItemSelectState", sol::overload(sol::resolve<ListboxItem*, bool>(&MultiColumnList::setItemSelectState), sol::resolve<const MCLGridRef&, bool>(&MultiColumnList::setItemSelectState)),
										"handleUpdatedItemData", &MultiColumnList::handleUpdatedItemData,
										"setColumnHeaderWidth", &MultiColumnList::setColumnHeaderWidth,
										"setUserSortControlEnabled", &MultiColumnList::setUserSortControlEnabled,
										"setUserColumnSizingEnabled", &MultiColumnList::setUserColumnSizingEnabled,
										"setUserColumnDraggingEnabled", &MultiColumnList::setUserColumnDraggingEnabled,
										"autoSizeColumnHeader", &MultiColumnList::autoSizeColumnHeader,
										"setRowID", &MultiColumnList::setRowID,
										"ensureRowIsVisible", &MultiColumnList::ensureRowIsVisible,
										"ensureColumnIsVisible", &MultiColumnList::ensureColumnIsVisible,
										"ensureItemRowIsVisible", &MultiColumnList::ensureItemRowIsVisible,
										"ensureItemColumnIsVisible", &MultiColumnList::ensureItemColumnIsVisible,
										"ensureItemIsVisible", sol::overload(sol::resolve<const ListboxItem*>(&MultiColumnList::ensureItemIsVisible), sol::resolve<const MCLGridRef&>(&MultiColumnList::ensureItemIsVisible)),
										"setAutoSizeColumnUsesHeader", &MultiColumnList::setAutoSizeColumnUsesHeader,
										"getAutoSizeColumnUsesHeader", &MultiColumnList::getAutoSizeColumnUsesHeader,
										sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<MultiLineEditbox>("MultiLineEditbox",
										 "hasInputFocus", &MultiLineEditbox::hasInputFocus,
										 "isReadOnly", &MultiLineEditbox::isReadOnly,
										 "getCaretIndex", &MultiLineEditbox::getCaretIndex,
										 "getSelectionStartIndex", &MultiLineEditbox::getSelectionStartIndex,
										 "getSelectionEndIndex", &MultiLineEditbox::getSelectionEndIndex,
										 "getSelectionLength", &MultiLineEditbox::getSelectionLength,
										 "getMaxTextLength", &MultiLineEditbox::getMaxTextLength,
										 "isWordWrapped", &MultiLineEditbox::isWordWrapped,
										 "getVertScrollbar", &MultiLineEditbox::getVertScrollbar,
										 "isVertScrollbarAlwaysShown", &MultiLineEditbox::isVertScrollbarAlwaysShown,
										 "getHorzScrollbar", &MultiLineEditbox::getHorzScrollbar,
										 "getTextRenderArea", &MultiLineEditbox::getTextRenderArea,
										 "getLineNumberFromIndex", &MultiLineEditbox::getLineNumberFromIndex,
										 "setReadOnly", &MultiLineEditbox::setReadOnly,
										 "setCaretIndex", &MultiLineEditbox::setCaretIndex,
										 "setSelection", &MultiLineEditbox::setSelection,
										 "setMaxTextLength", &MultiLineEditbox::setMaxTextLength,
										 "ensureCaretIsVisible", &MultiLineEditbox::ensureCaretIsVisible,
										 "setWordWrapping", &MultiLineEditbox::setWordWrapping,
										 "setShowVertScrollbar", &MultiLineEditbox::setShowVertScrollbar,
										 "setSelectionBrushImage", &MultiLineEditbox::setSelectionBrushImage,
										 "getSelectionBrushImage", &MultiLineEditbox::getSelectionBrushImage,
										 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);

	CEGUI.new_usertype<PopupMenu>("PopupMenu",
								  "getFadeInTime", &PopupMenu::getFadeInTime,
								  "getFadeOutTime", &PopupMenu::getFadeOutTime,
								  "setFadeInTime", &PopupMenu::setFadeInTime,
								  "setFadeOutTime", &PopupMenu::setFadeOutTime,
								  "openPopupMenu", sol::overload(&PopupMenu::openPopupMenu, [](PopupMenu* self) { self->openPopupMenu(); }),
								  "closePopupMenu", sol::overload(&PopupMenu::closePopupMenu, [](PopupMenu* self) { self->closePopupMenu(); }),
								  sol::base_classes, sol::bases<MenuBase, ItemListBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<ProgressBar>("ProgressBar",
									"getProgress", &ProgressBar::getProgress,
									"getStepSize", &ProgressBar::getStepSize,
									"setProgress", &ProgressBar::setProgress,
									"setStepSize", &ProgressBar::setStepSize,
									"step", &ProgressBar::step,
									"adjustProgress", &ProgressBar::adjustProgress,
									sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<RadioButton>("RadioButton",
									"getGroupID", &RadioButton::getGroupID,
									"setGroupID", &RadioButton::setGroupID,
									"getSelectedButtonInGroup", &RadioButton::getSelectedButtonInGroup,
									sol::base_classes, sol::bases<ToggleButton, ButtonBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<ScrollablePane>("ScrollablePane",
									   "getContentPane", &ScrollablePane::getContentPane,
									   "isVertScrollbarAlwaysShown", &ScrollablePane::isVertScrollbarAlwaysShown,
									   "setShowVertScrollbar", &ScrollablePane::setShowVertScrollbar,
									   "isHorzScrollbarAlwaysShown", &ScrollablePane::isHorzScrollbarAlwaysShown,
									   "setShowHorzScrollbar", &ScrollablePane::setShowHorzScrollbar,
									   "isContentPaneAutoSized", &ScrollablePane::isContentPaneAutoSized,
									   "setContentPaneAutoSized", &ScrollablePane::setContentPaneAutoSized,
									   "getContentPaneArea", &ScrollablePane::getContentPaneArea,
									   "setContentPaneArea", &ScrollablePane::setContentPaneArea,
									   "getHorizontalStepSize", &ScrollablePane::getHorizontalStepSize,
									   "setHorizontalStepSize", &ScrollablePane::setHorizontalStepSize,
									   "getHorizontalOverlapSize", &ScrollablePane::getHorizontalOverlapSize,
									   "setHorizontalOverlapSize", &ScrollablePane::setHorizontalOverlapSize,
									   "getHorizontalScrollPosition", &ScrollablePane::getHorizontalScrollPosition,
									   "setHorizontalScrollPosition", &ScrollablePane::setHorizontalScrollPosition,
									   "getVerticalStepSize", &ScrollablePane::getVerticalStepSize,
									   "setVerticalStepSize", &ScrollablePane::setVerticalStepSize,
									   "getVerticalOverlapSize", &ScrollablePane::getVerticalOverlapSize,
									   "setVerticalOverlapSize", &ScrollablePane::setVerticalOverlapSize,
									   "getVerticalScrollPosition", &ScrollablePane::getVerticalScrollPosition,
									   "setVerticalScrollPosition", &ScrollablePane::setVerticalScrollPosition,
									   "getViewableArea", &ScrollablePane::getViewableArea,
									   "getVertScrollbar", &ScrollablePane::getVertScrollbar,
									   "getHorzScrollbar", &ScrollablePane::getHorzScrollbar,
									   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<Scrollbar>("Scrollbar",
								  "getDocumentSize", &Scrollbar::getDocumentSize,
								  "getPageSize", &Scrollbar::getPageSize,
								  "getStepSize", &Scrollbar::getStepSize,
								  "getOverlapSize", &Scrollbar::getOverlapSize,
								  "getScrollPosition", &Scrollbar::getScrollPosition,
								  "getUnitIntervalScrollPosition", &Scrollbar::getUnitIntervalScrollPosition,
								  "isEndLockEnabled", &Scrollbar::isEndLockEnabled,
								  "setDocumentSize", &Scrollbar::setDocumentSize,
								  "setPageSize", &Scrollbar::setPageSize,
								  "setStepSize", &Scrollbar::setStepSize,
								  "setOverlapSize", &Scrollbar::setOverlapSize,
								  "setScrollPosition", &Scrollbar::setScrollPosition,
								  "setUnitIntervalScrollPosition", &Scrollbar::setUnitIntervalScrollPosition,
								  "setEndLockEnabled", &Scrollbar::setEndLockEnabled,
								  "scrollForwardsByStep", &Scrollbar::scrollForwardsByStep,
								  "scrollBackwardsByStep", &Scrollbar::scrollBackwardsByStep,
								  "scrollForwardsByPage", &Scrollbar::scrollForwardsByPage,
								  "scrollBackwardsByPage", &Scrollbar::scrollBackwardsByPage,
								  sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<ScrolledContainer>("ScrolledContainer",
										  "isContentPaneAutoSized", &ScrolledContainer::isContentPaneAutoSized,
										  "setContentPaneAutoSized", &ScrolledContainer::setContentPaneAutoSized,
										  "getContentArea", &ScrolledContainer::getContentArea,
										  "setContentArea", &ScrolledContainer::setContentArea,
										  "getChildExtentsArea", &ScrolledContainer::getChildExtentsArea,
										  sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<ScrolledItemListBase>("ScrolledItemListBase",
											 "isVertScrollbarAlwaysShown", &ScrolledItemListBase::isVertScrollbarAlwaysShown,
											 "isHorzScrollbarAlwaysShown", &ScrolledItemListBase::isHorzScrollbarAlwaysShown,
											 "getVertScrollbar", &ScrolledItemListBase::getVertScrollbar,
											 "getHorzScrollbar", &ScrolledItemListBase::getHorzScrollbar,
											 "setShowVertScrollbar", &ScrolledItemListBase::setShowVertScrollbar,
											 "setShowHorzScrollbar", &ScrolledItemListBase::setShowHorzScrollbar,
											 sol::base_classes, sol::bases<ItemListBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);

	CEGUI.new_usertype<SequentialLayoutContainer>("SequentialLayoutContainer",
												  "getPositionOfChild", sol::overload(
					sol::resolve<size_t(Window*) const>(&SequentialLayoutContainer::getPositionOfChild),
					[](SequentialLayoutContainer* self, const char* wnd) { return self->getPositionOfChild(wnd); }),
												  "getChildAtPosition", &SequentialLayoutContainer::getChildAtPosition,
												  "swapChildPositions", &SequentialLayoutContainer::swapChildPositions,
												  "swapChildren", sol::overload(sol::resolve<void(Window*, Window*)>(&SequentialLayoutContainer::swapChildren),
																				[](SequentialLayoutContainer* self, const char* wnd1, Window* wnd2) { self->swapChildren(wnd1, wnd2); },
																				[](SequentialLayoutContainer* self, const char* wnd1, const char* wnd2) { self->swapChildren(wnd1, wnd2); },
																				[](SequentialLayoutContainer* self, Window* wnd1, const char* wnd2) { self->swapChildren(wnd1, wnd2); }),
												  "moveChildToPosition", sol::overload(sol::resolve<void(Window*, size_t)>(&SequentialLayoutContainer::moveChildToPosition),
																					   [](SequentialLayoutContainer* self, const char* wnd, size_t position) { self->moveChildToPosition(wnd, position); }),
												  "addChildToPosition", &SequentialLayoutContainer::addChildToPosition,
												  "removeChildFromPosition", &SequentialLayoutContainer::removeChildFromPosition,
												  sol::base_classes, sol::bases<LayoutContainer, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<Slider>("Slider",
							   "getCurrentValue", &Slider::getCurrentValue,
							   "getMaxValue", &Slider::getMaxValue,
							   "getClickStep", &Slider::getClickStep,
							   "setMaxValue", &Slider::setMaxValue,
							   "setCurrentValue", &Slider::setCurrentValue,
							   "setClickStep", &Slider::setClickStep,
							   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<Spinner>("Spinner",
								"FloatingPoint", sol::var(Spinner::FloatingPoint),
								"Integer", sol::var(Spinner::Integer),
								"Hexadecimal", sol::var(Spinner::Hexadecimal),
								"Octal", sol::var(Spinner::Octal),
								"getCurrentValue", &Spinner::getCurrentValue,
								"getStepSize", &Spinner::getStepSize,
								"getMaximumValue", &Spinner::getMaximumValue,
								"getMinimumValue", &Spinner::getMinimumValue,
								"getTextInputMode", &Spinner::getTextInputMode,
								"setCurrentValue", &Spinner::setCurrentValue,
								"setStepSize", &Spinner::setStepSize,
								"setMaximumValue", &Spinner::setMaximumValue,
								"setMinimumValue", &Spinner::setMinimumValue,
								"setTextInputMode", &Spinner::setTextInputMode,
								sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<TabButton>("TabButton",
								  "isSelected", &TabButton::isSelected,
								  "setSelected", &TabButton::setSelected,
								  "setTargetWindow", &TabButton::setTargetWindow,
								  "getTargetWindow", &TabButton::getTargetWindow,
								  sol::base_classes, sol::bases<ButtonBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);

	CEGUI.new_usertype<TabControl>("TabControl",
								   "getTabCount", &TabControl::getTabCount,
								   "setSelectedTab", sol::overload([](TabControl* self, const char* name) { self->setSelectedTab(name); }, sol::resolve<void(unsigned int)>(&TabControl::setSelectedTab)),
								   "setSelectedTabAtIndex", &TabControl::setSelectedTabAtIndex,
								   "getTabContentsAtIndex", &TabControl::getTabContentsAtIndex,
								   "getTabContents", sol::overload([](TabControl* self, const char* name) { return self->getTabContents(name); }, sol::resolve<Window*(unsigned int) const>(&TabControl::getTabContents)),
								   "isTabContentsSelected", &TabControl::isTabContentsSelected,
								   "getSelectedTabIndex", &TabControl::getSelectedTabIndex,
								   "getTabHeight", &TabControl::getTabHeight,
								   "getTabTextPadding", &TabControl::getTabTextPadding,
								   "setTabHeight", &TabControl::setTabHeight,
								   "setTabTextPadding", &TabControl::setTabTextPadding,
								   "addTab", &TabControl::addTab,
								   "removeTab", sol::overload([](TabControl* self, const char* name) { self->removeTab(name); }, sol::resolve<void(unsigned int)>(&TabControl::removeTab)),
								   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);

	CEGUI.new_usertype<Thumb>("Thumb",
							  "isHotTracked", &Thumb::isHotTracked,
							  "isVertFree", &Thumb::isVertFree,
							  "isHorzFree", &Thumb::isHorzFree,
							  "setHotTracked", &Thumb::setHotTracked,
							  "setVertFree", &Thumb::setVertFree,
							  "setHorzFree", &Thumb::setHorzFree,
							  "setVertRange", sol::resolve<void(float
																min, float
																max)>(&Thumb::setVertRange),
							  "setHorzRange", sol::resolve<void(float
																min, float
																max)>(&Thumb::setHorzRange),
							  "getVertRange", [](Thumb* self) {
				auto range = self->getVertRange();
				return std::tuple(range.first, range.second);
			},
							  "getHorzRange", [](Thumb* self) {
				auto range = self->getHorzRange();
				return std::tuple(range.first, range.second);
			},
							  sol::base_classes, sol::bases<PushButton, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<Titlebar>("Titlebar",
								 "isDraggingEnabled", &Titlebar::isDraggingEnabled,
								 "setDraggingEnabled", &Titlebar::setDraggingEnabled,
								 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<Tooltip>("Tooltip",
								"setTargetWindow", &Tooltip::setTargetWindow,
								"getTargetWindow", &Tooltip::getTargetWindow,
								"resetTimer", &Tooltip::resetTimer,
								"setHoverTime", &Tooltip::setHoverTime,
								"getHoverTime", &Tooltip::getHoverTime,
								"setDisplayTime", &Tooltip::setDisplayTime,
								"getDisplayTime", &Tooltip::getDisplayTime,
								"positionSelf", &Tooltip::positionSelf,
								"sizeSelf", &Tooltip::sizeSelf,
								"getTextSize", &Tooltip::getTextSize,
								sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<Tree>("Tree",
							 "getItemCount", &Tree::getItemCount,
							 "getSelectedCount", &Tree::getSelectedCount,
							 "getFirstSelectedItem", &Tree::getFirstSelectedItem,
							 "getLastSelectedItem", &Tree::getLastSelectedItem,
							 "isSortEnabled", &Tree::isSortEnabled,
							 "isMultiselectEnabled", &Tree::isMultiselectEnabled,
							 "isItemTooltipsEnabled", &Tree::isItemTooltipsEnabled,
							 "findFirstItemWithText", [](Tree* self, const char* text) { return self->findFirstItemWithText(text); },
							 "findNextItemWithText", [](Tree* self, const char* text, const TreeItem* start_item) { return self->findNextItemWithText(text, start_item); },
							 "findFirstItemWithID", &Tree::findFirstItemWithID,
							 "findNextItemWithID", &Tree::findNextItemWithID,
							 "isTreeItemInList", &Tree::isTreeItemInList,
							 "resetList", &Tree::resetList,
							 "addItem", &Tree::addItem,
							 "insertItem", &Tree::insertItem,
							 "removeItem", &Tree::removeItem,
							 "clearAllSelections", &Tree::clearAllSelections,
							 "setSortingEnabled", &Tree::setSortingEnabled,
							 "setMultiselectEnabled", &Tree::setMultiselectEnabled,
							 "setItemSelectState", sol::overload(sol::resolve<void(TreeItem*, bool)>(&Tree::setItemSelectState),
																 sol::resolve<void(size_t, bool)>(&Tree::setItemSelectState)),
							 "ensureItemIsVisible", &Tree::ensureItemIsVisible,
							 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<TreeItem>("TreeItem",
								 "getFont", &TreeItem::getFont,
								 "getTextColours", &TreeItem::getTextColours,
								 "setFont", sol::overload(sol::resolve<void(
																  const Font*)>(&TreeItem::setFont),
														  [](TreeItem* self, const char* font_name) { self->setFont(font_name); }),
								 "setTextColours", sol::overload(sol::resolve<void(
																		 const ColourRect&)>(&TreeItem::setTextColours),
																 sol::resolve<void(Colour, Colour, Colour, Colour)>(&TreeItem::setTextColours),
																 sol::resolve<void(Colour)>(&TreeItem::setTextColours)),
								 "getText", [](TreeItem* self) { return std::string(self->getText().c_str()); },
								 "getTooltipText", [](TreeItem* self) { return std::string(self->getTooltipText().c_str()); },
								 "getID", &TreeItem::getID,
								 "getUserData", &TreeItem::getUserData,
								 "isSelected", &TreeItem::isSelected,
								 "isDisabled", &TreeItem::isDisabled,
								 "isAutoDeleted", &TreeItem::isAutoDeleted,
								 "getOwnerWindow", &TreeItem::getOwnerWindow,
								 "getSelectionColours", &TreeItem::getSelectionColours,
								 "getSelectionBrushImage", &TreeItem::getSelectionBrushImage,
								 "setText", string_setter(&TreeItem::setText),
								 "setTooltipText", string_setter(&TreeItem::setTooltipText),
								 "setID", &TreeItem::setID,
								 "setUserData", &TreeItem::setUserData,
								 "setSelected", &TreeItem::setSelected,
								 "setDisabled", &TreeItem::setDisabled,
								 "setAutoDeleted", &TreeItem::setAutoDeleted,
								 "setOwnerWindow", &TreeItem::setOwnerWindow,
								 "setSelectionColours", sol::overload(sol::resolve<void(
																			  const ColourRect&)>(&TreeItem::setSelectionColours),
																	  sol::resolve<void(Colour, Colour, Colour, Colour)>(&TreeItem::setSelectionColours),
																	  sol::resolve<void(Colour)>(&TreeItem::setSelectionColours)),
								 "setSelectionBrushImage", sol::overload(sol::resolve<const Image*>(&TreeItem::setSelectionBrushImage), [](TreeItem* self, const char* name) { self->setSelectionBrushImage(name); }),
								 "setButtonLocation", &TreeItem::setButtonLocation,
								 "getButtonLocation", &TreeItem::getButtonLocation,
								 "getIsOpen", &TreeItem::getIsOpen,
								 "toggleIsOpen", &TreeItem::toggleIsOpen,
								 "getTreeItemFromIndex", &TreeItem::getTreeItemFromIndex,
								 "getItemCount", &TreeItem::getItemCount,
								 "addItem", &TreeItem::addItem,
								 "removeItem", &TreeItem::removeItem,
								 "setIcon", &TreeItem::setIcon
	);
	CEGUI.new_usertype<VerticalLayoutContainer>("VerticalLayoutContainer",
												sol::base_classes, sol::bases<SequentialLayoutContainer, LayoutContainer, Window, NamedElement, Element, PropertySet, EventSet>()
	);

	auto imagePair = CEGUI.new_usertype<ImageManager::ImagePair>("ImagePair");
	imagePair["first"] = &ImageManager::ImagePair::first;
	imagePair["second"] = &ImageManager::ImagePair::second;

	auto imageManager = CEGUI.new_usertype<ImageManager>("ImageManager");
	imageManager["getSingleton"] = &ImageManager::getSingleton;
//	imageManager["getIterator"] = [](ImageManager* self) { return std::unique_ptr<ImageManager::ImageIterator>(new ImageManager::ImageIterator(self->getIterator())); };
	imageManager["getIterator"] = &ImageManager::getIterator;
	imageManager["get"] = string_setter(&ImageManager::get);

	auto imageIterator = CEGUI.new_usertype<ImageManager::ImageIterator>("ImageIterator");
	imageIterator["key"] = string_getter(&ImageManager::ImageIterator::getCurrentKey);
	imageIterator["value"] = &ImageManager::ImageIterator::getCurrentValue;
	imageIterator["isAtEnd"] = &ImageManager::ImageIterator::isAtEnd;
	imageIterator["isAtStart"] = &ImageManager::ImageIterator::isAtStart;
	imageIterator["next"] = [](ImageManager::ImageIterator* self) {
		(*self)++;
		return self;
	};
	imageIterator["previous"] = [](ImageManager::ImageIterator* self) {
		(*self)--;
		return self;
	};
	imageIterator["toStart"] = &ImageManager::ImageIterator::toStart;
	imageIterator["toEnd"] = &ImageManager::ImageIterator::toEnd;

	auto image = CEGUI.new_usertype<Image>("Image");
	image["getName"] = [](Image* self) { return std::string(self->getName().c_str()); };

	auto mouseCursor = CEGUI.new_usertype<MouseCursor>("MouseCursor",
													   sol::base_classes, sol::bases<EventSet>()
	);
	mouseCursor["getPosition"] = &MouseCursor::getPosition;
	mouseCursor["hide"] = &MouseCursor::hide;
	mouseCursor["show"] = &MouseCursor::show;
	mouseCursor["setVisible"] = &MouseCursor::setVisible;
	mouseCursor["isVisible"] = &MouseCursor::isVisible;

	auto vector2f = CEGUI.new_usertype<Vector2<float >>("Vector2f",
														sol::constructors<Vector2<float>(), Vector2<float>(float, float)>());
	vector2f["x"] = &Vector2<float>::d_x;
	vector2f["y"] = &Vector2<float>::d_y;

	auto vector3f = CEGUI.new_usertype<Vector3<float >>("Vector3f",
														sol::constructors<Vector2<float>(), Vector2<float>(float, float, float)>());
	vector3f["x"] = &Vector3<float>::d_x;
	vector3f["y"] = &Vector3<float>::d_y;
	vector3f["z"] = &Vector3<float>::d_z;

	auto sizef = CEGUI.new_usertype<Size<float >>("Sizef",
												  sol::constructors<Size<float>(), Size<float>(float, float)>());
	sizef["width"] = &Size<float>::d_width;
	sizef["height"] = &Size<float>::d_height;


	auto propertyHelper = lua.create_table();
	propertyHelper["stringToFloat"] = [](const char* str) { return PropertyHelper<float>::fromString(str); };
	propertyHelper["stringToUint"] = [](const char* str) { return PropertyHelper<unsigned int>::fromString(str); };
	propertyHelper["stringToBool"] = [](const char* str) { return PropertyHelper<bool>::fromString(str); };
	propertyHelper["stringToSize"] = [](const char* str) { return PropertyHelper<Sizef>::fromString(str); };
	propertyHelper["stringToVector2"] = [](const char* str) { return PropertyHelper<Vector2f>::fromString(str); };
	propertyHelper["stringToRect"] = [](const char* str) { return PropertyHelper<Rectf>::fromString(str); };
	propertyHelper["stringToImage"] = [](const char* str) { return PropertyHelper<const Image*>::fromString(str); };
	propertyHelper["stringToColour"] = [](const char* str) { return PropertyHelper<Colour>::fromString(str); };
	propertyHelper["stringToColourRect"] = [](const char* str) { return PropertyHelper<ColourRect>::fromString(str); };
	propertyHelper["stringToUDim"] = [](const char* str) { return PropertyHelper<UDim>::fromString(str); };
	propertyHelper["stringToUVector2"] = [](const char* str) { return PropertyHelper<UVector2>::fromString(str); };
	propertyHelper["stringToURect"] = [](const char* str) { return PropertyHelper<URect>::fromString(str); };
	propertyHelper["stringToAspectMode"] = [](const char* str) { return PropertyHelper<AspectMode>::fromString(str); };
	propertyHelper["floatToString"] = [](float val) { return std::string(PropertyHelper<float>::toString(val).c_str()); };
	propertyHelper["uintToString"] = [](unsigned int val) { return std::string(PropertyHelper<unsigned int>::toString(val).c_str()); };
	propertyHelper["boolToString"] = [](bool val) { return std::string(PropertyHelper<bool>::toString(val).c_str()); };
	propertyHelper["sizeToString"] = [](const Sizef& val) { return std::string(PropertyHelper<Sizef>::toString(val).c_str()); };
	propertyHelper["vector2ToString"] = [](const Vector2f& val) { return std::string(PropertyHelper<Vector2f>::toString(val).c_str()); };
	propertyHelper["rectToString"] = [](const Rectf& val) { return std::string(PropertyHelper<Rectf>::toString(val).c_str()); };
	propertyHelper["imageToString"] = [](const Image* val) { return std::string(PropertyHelper<const Image*>::toString(val).c_str()); };
	propertyHelper["colourToString"] = [](const Colour& val) { return std::string(PropertyHelper<Colour>::toString(val).c_str()); };
	propertyHelper["colourRectToString"] = [](const ColourRect& val) { return std::string(PropertyHelper<ColourRect>::toString(val).c_str()); };
	propertyHelper["udimToString"] = [](const UDim& val) { return std::string(PropertyHelper<UDim>::toString(val).c_str()); };
	propertyHelper["uvector2ToString"] = [](const UVector2& val) { return std::string(PropertyHelper<UVector2>::toString(val).c_str()); };
	propertyHelper["usizeToString"] = [](const USize& val) { return std::string(PropertyHelper<USize>::toString(val).c_str()); };
	propertyHelper["urectToString"] = [](const URect& val) { return std::string(PropertyHelper<URect>::toString(val).c_str()); };
	propertyHelper["aspectModeToString"] = [](AspectMode val) { return std::string(PropertyHelper<AspectMode>::toString(val).c_str()); };
	CEGUI["PropertyHelper"] = propertyHelper;


	CEGUI["toButtonBase"] = [](Window* self) { return dynamic_cast<ButtonBase*>(self); };
	CEGUI["toCheckbox"] = [](Window* self) { return dynamic_cast<ToggleButton*>(self); };
	CEGUI["toCombobox"] = [](Window* self) { return dynamic_cast<Combobox*>(self); };
	CEGUI["toComboDropList"] = [](Window* self) { return dynamic_cast<ComboDropList*>(self); };
	CEGUI["toDragContainer"] = [](Window* self) { return dynamic_cast<DragContainer*>(self); };
	CEGUI["toEditbox"] = [](Window* self) { return dynamic_cast<Editbox*>(self); };
	CEGUI["toFrameWindow"] = [](Window* self) { return dynamic_cast<FrameWindow*>(self); };
	CEGUI["toGridLayoutContainer"] = [](Window* self) { return dynamic_cast<GridLayoutContainer*>(self); };
	CEGUI["toHorizontalLayoutContainer"] = [](Window* self) { return dynamic_cast<HorizontalLayoutContainer*>(self); };
	CEGUI["toItemEntry"] = [](Window* self) { return dynamic_cast<ItemEntry*>(self); };
	CEGUI["toItemListBase"] = [](Window* self) { return dynamic_cast<ItemListBase*>(self); };
	CEGUI["toItemListbox"] = [](Window* self) { return dynamic_cast<ItemListbox*>(self); };
	CEGUI["toLayoutContainer"] = [](Window* self) { return dynamic_cast<LayoutContainer*>(self); };
	CEGUI["toListbox"] = [](Window* self) { return dynamic_cast<Listbox*>(self); };
	CEGUI["toListHeader"] = [](Window* self) { return dynamic_cast<ListHeader*>(self); };
	CEGUI["toListHeaderSegment"] = [](Window* self) { return dynamic_cast<ListHeaderSegment*>(self); };
	CEGUI["toMenubar"] = [](Window* self) { return dynamic_cast<Menubar*>(self); };
	CEGUI["toMenuBase"] = [](Window* self) { return dynamic_cast<MenuBase*>(self); };
	CEGUI["toMenuItem"] = [](Window* self) { return dynamic_cast<MenuItem*>(self); };
	CEGUI["toMultiColumnList"] = [](Window* self) { return dynamic_cast<MultiColumnList*>(self); };
	CEGUI["toMultiLineEditbox"] = [](Window* self) { return dynamic_cast<MultiLineEditbox*>(self); };
	CEGUI["toPopupMenu"] = [](Window* self) { return dynamic_cast<PopupMenu*>(self); };
	CEGUI["toProgressBar"] = [](Window* self) { return dynamic_cast<ProgressBar*>(self); };
	CEGUI["toPushButton"] = [](Window* self) { return dynamic_cast<PushButton*>(self); };
	CEGUI["toRadioButton"] = [](Window* self) { return dynamic_cast<RadioButton*>(self); };
	CEGUI["toScrollablePane"] = [](Window* self) { return dynamic_cast<ScrollablePane*>(self); };
	CEGUI["toScrollbar"] = [](Window* self) { return dynamic_cast<Scrollbar*>(self); };
	CEGUI["toScrolledContainer"] = [](Window* self) { return dynamic_cast<ScrolledContainer*>(self); };
	CEGUI["toScrolledItemListBase"] = [](Window* self) { return dynamic_cast<ScrolledItemListBase*>(self); };
	CEGUI["toSequentialLayoutContainer"] = [](Window* self) { return dynamic_cast<SequentialLayoutContainer*>(self); };
	CEGUI["toSlider"] = [](Window* self) { return dynamic_cast<Slider*>(self); };
	CEGUI["toSpinner"] = [](Window* self) { return dynamic_cast<Spinner*>(self); };
	CEGUI["toTabButton"] = [](Window* self) { return dynamic_cast<TabButton*>(self); };
	CEGUI["toTabControl"] = [](Window* self) { return dynamic_cast<TabControl*>(self); };
	CEGUI["toThumb"] = [](Window* self) { return dynamic_cast<Thumb*>(self); };
	CEGUI["toToggleButton"] = [](Window* self) { return dynamic_cast<ToggleButton*>(self); };
	CEGUI["toTooltip"] = [](Window* self) { return dynamic_cast<Tooltip*>(self); };
	CEGUI["toTree"] = [](Window* self) { return dynamic_cast<Tree*>(self); };
	CEGUI["toVerticalLayoutContainer"] = [](Window* self) { return dynamic_cast<VerticalLayoutContainer*>(self); };

}
