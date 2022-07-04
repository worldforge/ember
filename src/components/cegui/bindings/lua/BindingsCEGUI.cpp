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

struct LuaFunctor : std::enable_shared_from_this<LuaFunctor> {

	sol::function callback;

	LuaFunctor(sol::function _callback) : callback(_callback) {}

	bool handle(const EventArgs& eventArgs) {
		//Create a reference to ourselves, so we'll complete even if Lua garbage collects us.
		auto self = this->shared_from_this();

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
	}
};

struct LuaFunctorWithSelf : std::enable_shared_from_this<LuaFunctorWithSelf> {

	sol::function callback;
	sol::table this_object;

	LuaFunctorWithSelf(sol::function _callback, sol::table _this_object)
			: callback(_callback),
			  this_object(_this_object) {}

	bool handle(const EventArgs& eventArgs) {
		//Create a reference to ourselves, so we'll complete even if Lua garbage collects us.
		auto self = this->shared_from_this();

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
	}
};

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

	auto connection = Cegui.new_usertype<Event::Connection>("Connection");
	connection["disconnect"] = [](Event::Connection* self) { (*self)->disconnect(); };


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

	auto udim = CEGUI.new_usertype<UDim>("UDim",
										 sol::constructors<UDim(), UDim(float, float)>(),
										 "scale", sol::property(&UDim::d_scale),
										 "offset", sol::property(&UDim::d_offset)
	);
	udim[sol::meta_method::addition] = [](UDim* lhs, UDim* rhs) { return *lhs + *rhs; };
	udim[sol::meta_method::subtraction] = [](UDim* lhs, UDim* rhs) { return *lhs - *rhs; };

	auto vector = CEGUI.new_usertype<UVector2>("UVector2",
											   sol::constructors<UVector2(), UVector2(UDim, UDim)>());
	vector["x"] = sol::property(&UVector2::d_x);
	vector["y"] = sol::property(&UVector2::d_y);


	auto usize = CEGUI.new_usertype<USize>("USize",
										   sol::constructors<UVector2(), UVector2(UDim, UDim)>()
	);
	usize["width"] = sol::property(&USize::d_width);
	usize["height"] = sol::property(&USize::d_height);

	auto urect = CEGUI.new_usertype<URect>("URect",
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

	auto ubox = CEGUI.new_usertype<UBox>("UBox",
										 sol::constructors<UBox(), UBox(
												 const UDim&), UBox(
												 const UDim&, const UDim&, const UDim&, const UDim&), UBox(
												 const UBox&)>());
	ubox["top"] = sol::property(&UBox::d_top);
	ubox["left"] = sol::property(&UBox::d_left);
	ubox["bottom"] = sol::property(&UBox::d_bottom);
	ubox["right"] = sol::property(&UBox::d_right);

	CEGUI.new_usertype<EventArgs>("EventArgs",
								  "handled", sol::property(&EventArgs::handled)
	);

	CEGUI.new_usertype<MouseCursorEventArgs>("MouseCursorEventArgs",
											 "mouseCursor", sol::property(&MouseCursorEventArgs::mouseCursor),
											 "image", sol::property(&MouseCursorEventArgs::image),
											 sol::base_classes, sol::bases<EventArgs>()
	);

	CEGUI.new_usertype<WindowEventArgs>("WindowEventArgs",
										sol::constructors<WindowEventArgs(Window*)>(),
										"window", sol::property(&WindowEventArgs::window),
										sol::base_classes, sol::bases<EventArgs>()
	);

	CEGUI.new_usertype<ActivationEventArgs>("ActivationEventArgs",
											"otherWindow", sol::property(&ActivationEventArgs::otherWindow),
											sol::base_classes, sol::bases<WindowEventArgs, EventArgs>()
	);
	CEGUI.new_usertype<HeaderSequenceEventArgs>("HeaderSequenceEventArgs",
												"oldIdx", sol::property(&HeaderSequenceEventArgs::d_oldIdx),
												"newIdx", sol::property(&HeaderSequenceEventArgs::d_newIdx),
												sol::base_classes, sol::bases<WindowEventArgs, EventArgs>()
	);

	CEGUI["LeftButton"] = MouseButton::LeftButton;
	CEGUI["RightButton"] = MouseButton::RightButton;
	CEGUI["MiddleButton"] = MouseButton::MiddleButton;
	CEGUI["X1Button"] = MouseButton::X1Button;
	CEGUI["X2Button"] = MouseButton::X2Button;
	CEGUI["MouseButtonCount"] = MouseButton::MouseButtonCount;
	CEGUI["NoButton"] = MouseButton::NoButton;

	auto mouseEventArgs = CEGUI.new_usertype<MouseEventArgs>("MouseEventArgs",
															 sol::base_classes, sol::bases<WindowEventArgs, EventArgs>());
	mouseEventArgs["position"] = sol::property(&MouseEventArgs::position);
	mouseEventArgs["moveDelta"] = sol::property(&MouseEventArgs::moveDelta);
	mouseEventArgs["button"] = sol::property(&MouseEventArgs::button);
	mouseEventArgs["sysKeys"] = sol::property(&MouseEventArgs::sysKeys);
	mouseEventArgs["wheelChange"] = sol::property(&MouseEventArgs::wheelChange);

	auto keyEventArgs = CEGUI.new_usertype<KeyEventArgs>("KeyEventArgs",
														 sol::base_classes, sol::bases<WindowEventArgs, EventArgs>()
	);
	keyEventArgs["codepoint"] = sol::property(&KeyEventArgs::codepoint);
	keyEventArgs["scancode"] = sol::property(&KeyEventArgs::scancode);
	keyEventArgs["sysKeys"] = sol::property(&KeyEventArgs::sysKeys);

	CEGUI.new_usertype<DragDropEventArgs>("DragDropEventArgs",
										  "dragDropItem", sol::property(&DragDropEventArgs::dragDropItem),
										  sol::base_classes, sol::bases<WindowEventArgs, EventArgs>()
	);

	CEGUI.new_usertype<TreeEventArgs>("TreeEventArgs",
									  "treeItem", sol::property(&TreeEventArgs::treeItem),
									  sol::base_classes, sol::bases<WindowEventArgs, EventArgs>()
	);

	CEGUI.new_usertype<RenderQueueEventArgs>("RenderQueueEventArgs",
											 "queueID", sol::property(&RenderQueueEventArgs::queueID),
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
	combobox["findItemWithText"] = [](Combobox* self, const char* text, const ListboxItem* start_item) { return self->findItemWithText(text, start_item); };
	combobox["isListboxItemInList"] = &Combobox::isListboxItemInList;
	combobox["resetList"] = &Combobox::resetList;
	combobox["addItem"] = &Combobox::addItem;
	combobox["insertItem"] = &Combobox::insertItem;
	combobox["removeItem"] = &Combobox::removeItem;
	combobox["clearAllSelections"] = &Combobox::clearAllSelections;
	combobox["setSortingEnabled"] = &Combobox::setSortingEnabled;
	combobox["setShowVertScrollbar"] = &Combobox::setShowVertScrollbar;
	combobox["setShowHorzScrollbar"] = &Combobox::setShowHorzScrollbar;
	combobox["setItemSelectState"] = sol::overload(sol::resolve<void(ListboxItem*, bool)>(&Combobox::setItemSelectState), sol::resolve<void(size_t, bool)>(&Combobox::setItemSelectState));
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

	auto comboDropList = CEGUI.new_usertype<ComboDropList>("ComboDropList",
														   sol::base_classes, sol::bases<Listbox, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	comboDropList["setArmed"] = &ComboDropList::setArmed;
	comboDropList["isArmed"] = &ComboDropList::isArmed;
	comboDropList["setAutoArmEnabled"] = &ComboDropList::setAutoArmEnabled;
	comboDropList["isAutoArmEnabled"] = &ComboDropList::isAutoArmEnabled;
	comboDropList["resizeToContent"] = &ComboDropList::resizeToContent;

	auto dragContainer = CEGUI.new_usertype<DragContainer>("DragContainer",
														   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	dragContainer["isDraggingEnabled"] = &DragContainer::isDraggingEnabled;
	dragContainer["setDraggingEnabled"] = &DragContainer::setDraggingEnabled;
	dragContainer["isBeingDragged"] = &DragContainer::isBeingDragged;
	dragContainer["getPixelDragThreshold"] = &DragContainer::getPixelDragThreshold;
	dragContainer["setPixelDragThreshold"] = &DragContainer::setPixelDragThreshold;
	dragContainer["getDragAlpha"] = &DragContainer::getDragAlpha;
	dragContainer["setDragAlpha"] = &DragContainer::setDragAlpha;
	dragContainer["getDragCursorImage"] = &DragContainer::getDragCursorImage;
	dragContainer["setDragCursorImage"] = sol::overload([](DragContainer* self, const char* name) { self->setDragCursorImage(name); }, sol::resolve<const Image*>(&DragContainer::setDragCursorImage));
	dragContainer["getCurrentDropTarget"] = &DragContainer::getCurrentDropTarget;
	dragContainer["isStickyModeEnabled"] = &DragContainer::isStickyModeEnabled;
	dragContainer["setStickyModeEnabled"] = &DragContainer::setStickyModeEnabled;
	dragContainer["pickUp"] = &DragContainer::pickUp;
	dragContainer["setFixedDragOffset"] = &DragContainer::setFixedDragOffset;
	dragContainer["getFixedDragOffset"] = &DragContainer::getFixedDragOffset;
	dragContainer["setUsingFixedDragOffset"] = &DragContainer::setUsingFixedDragOffset;
	dragContainer["isUsingFixedDragOffset"] = &DragContainer::isUsingFixedDragOffset;


	auto editBox = CEGUI.new_usertype<Editbox>("Editbox",
											   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	editBox["hasInputFocus"] = &Editbox::hasInputFocus;
	editBox["isReadOnly"] = &Editbox::isReadOnly;
	editBox["isTextMasked"] = &Editbox::isTextMasked;
	editBox["getTextMatchState"] = &Editbox::getTextMatchState;
	editBox["getValidationString"] = string_getter(&Editbox::getValidationString);
	editBox["getCaretIndex"] = &Editbox::getCaretIndex;
	editBox["getSelectionStartIndex"] = &Editbox::getSelectionStartIndex;
	editBox["getSelectionEndIndex"] = &Editbox::getSelectionEndIndex;
	editBox["getSelectionLength"] = &Editbox::getSelectionLength;
	editBox["getMaskCodePoint"] = &Editbox::getMaskCodePoint;
	editBox["getMaxTextLength"] = &Editbox::getMaxTextLength;
	editBox["setReadOnly"] = &Editbox::setReadOnly;
	editBox["setTextMasked"] = &Editbox::setTextMasked;
	editBox["setValidationString"] = string_setter(&Editbox::setValidationString);
	editBox["setCaretIndex"] = &Editbox::setCaretIndex;
	editBox["setSelection"] = &Editbox::setSelection;
	editBox["setMaskCodePoint"] = &Editbox::setMaskCodePoint;
	editBox["setMaxTextLength"] = &Editbox::setMaxTextLength;


	auto frameWindow = CEGUI.new_usertype<FrameWindow>("FrameWindow",
													   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	frameWindow["isSizingEnabled"] = &FrameWindow::isSizingEnabled;
	frameWindow["isFrameEnabled"] = &FrameWindow::isFrameEnabled;
	frameWindow["isTitleBarEnabled"] = &FrameWindow::isTitleBarEnabled;
	frameWindow["isCloseButtonEnabled"] = &FrameWindow::isCloseButtonEnabled;
	frameWindow["isRollupEnabled"] = &FrameWindow::isRollupEnabled;
	frameWindow["isRolledup"] = &FrameWindow::isRolledup;
	frameWindow["isDragMovingEnabled"] = &FrameWindow::isDragMovingEnabled;
	frameWindow["setSizingEnabled"] = &FrameWindow::setSizingEnabled;
	frameWindow["setFrameEnabled"] = &FrameWindow::setFrameEnabled;
	frameWindow["setTitleBarEnabled"] = &FrameWindow::setTitleBarEnabled;
	frameWindow["setCloseButtonEnabled"] = &FrameWindow::setCloseButtonEnabled;
	frameWindow["setRollupEnabled"] = &FrameWindow::setRollupEnabled;
	frameWindow["setDragMovingEnabled"] = &FrameWindow::setDragMovingEnabled;
	frameWindow["getSizingBorderThickness"] = &FrameWindow::getSizingBorderThickness;
	frameWindow["setSizingBorderThickness"] = &FrameWindow::setSizingBorderThickness;
	frameWindow["toggleRollup"] = &FrameWindow::toggleRollup;
	frameWindow["offsetPixelPosition"] = &FrameWindow::offsetPixelPosition;

	auto gridLayoutContainer = CEGUI.new_usertype<GridLayoutContainer>("GridLayoutContainer",
																	   sol::base_classes, sol::bases<LayoutContainer, Window, NamedElement, Element, PropertySet, EventSet>()
	);
//											"AP_Disabled", GridLayoutContainer::AP_Disabled,
//											"AP_LeftToRight", GridLayoutContainer::AP_LeftToRight,
//											"AP_TopToBottom", GridLayoutContainer::AP_TopToBottom,
	gridLayoutContainer["setGridDimensions"] = &GridLayoutContainer::setGridDimensions;
	gridLayoutContainer["getGridWidth"] = &GridLayoutContainer::getGridWidth;
	gridLayoutContainer["getGridHeight"] = &GridLayoutContainer::getGridHeight;
	gridLayoutContainer["setAutoPositioning"] = &GridLayoutContainer::setAutoPositioning;
	gridLayoutContainer["getAutoPositioning"] = &GridLayoutContainer::getAutoPositioning;
	gridLayoutContainer["setNextAutoPositioningIdx"] = &GridLayoutContainer::setNextAutoPositioningIdx;
	gridLayoutContainer["getNextAutoPositioningIdx"] = &GridLayoutContainer::getNextAutoPositioningIdx;
	gridLayoutContainer["autoPositioningSkipCells"] = &GridLayoutContainer::autoPositioningSkipCells;
	gridLayoutContainer["addChildToPosition"] = &GridLayoutContainer::addChildToPosition;
	gridLayoutContainer["getChildAtPosition"] = &GridLayoutContainer::getChildAtPosition;
	gridLayoutContainer["removeChildFromPosition"] = &GridLayoutContainer::removeChildFromPosition;
	gridLayoutContainer["swapChildPositions"] = sol::resolve<size_t, size_t, size_t, size_t>(&GridLayoutContainer::swapChildPositions);
	gridLayoutContainer["swapChildren"] = sol::resolve<Window*, Window*>(&GridLayoutContainer::swapChildren);
	gridLayoutContainer["moveChildToPosition"] = sol::resolve<Window*, size_t, size_t>(&GridLayoutContainer::moveChildToPosition);


	CEGUI.new_usertype<GroupBox>("GroupBox",
								 "drawAroundWidget", [](GroupBox* self, const char* name) { return self->drawAroundWidget(name); },
								 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	CEGUI.new_usertype<HorizontalLayoutContainer>("HorizontalLayoutContainer",
												  sol::base_classes, sol::bases<SequentialLayoutContainer, LayoutContainer, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	auto itemEntry = CEGUI.new_usertype<ItemEntry>("ItemEntry",
												   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	itemEntry["getItemPixelSize"] = &ItemEntry::getItemPixelSize;
	itemEntry["getOwnerList"] = &ItemEntry::getOwnerList;
	itemEntry["isSelected"] = &ItemEntry::isSelected;
	itemEntry["isSelectable"] = &ItemEntry::isSelectable;
	itemEntry["setSelected"] = &ItemEntry::setSelected;
	itemEntry["select"] = &ItemEntry::select;
	itemEntry["deselect"] = &ItemEntry::deselect;
	itemEntry["setSelectable"] = &ItemEntry::setSelectable;


	auto itemListBase = CEGUI.new_usertype<ItemListBase>("ItemListBase",
														 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	itemListBase["Ascending"] = sol::var(ItemListBase::Ascending);
	itemListBase["Descending"] = sol::var(ItemListBase::Descending);
	itemListBase["UserSort"] = sol::var(ItemListBase::UserSort);
	itemListBase["getItemCount"] = &ItemListBase::getItemCount;
	itemListBase["getItemFromIndex"] = &ItemListBase::getItemFromIndex;
	itemListBase["getItemIndex"] = &ItemListBase::getItemIndex;
	itemListBase["findItemWithText"] = [](ItemListBase* self, const char* text, const ItemEntry* start_item) { return self->findItemWithText(text, start_item); };
	itemListBase["isItemInList"] = &ItemListBase::isItemInList;
	itemListBase["isAutoResizeEnabled"] = &ItemListBase::isAutoResizeEnabled;
	itemListBase["isSortEnabled"] = &ItemListBase::isSortEnabled;
	itemListBase["getSortMode"] = &ItemListBase::getSortMode;
	itemListBase["getItemRenderArea"] = &ItemListBase::getItemRenderArea;
	itemListBase["getContentPane"] = &ItemListBase::getContentPane;
	itemListBase["resetList"] = &ItemListBase::resetList;
	itemListBase["addItem"] = &ItemListBase::addItem;
	itemListBase["insertItem"] = &ItemListBase::insertItem;
	itemListBase["removeItem"] = &ItemListBase::removeItem;
	itemListBase["handleUpdatedItemData"] = &ItemListBase::handleUpdatedItemData;
	itemListBase["setAutoResizeEnabled"] = &ItemListBase::setAutoResizeEnabled;
	itemListBase["sizeToContent"] = &ItemListBase::sizeToContent;
	itemListBase["setSortEnabled"] = &ItemListBase::setSortEnabled;
	itemListBase["setSortMode"] = &ItemListBase::setSortMode;
	itemListBase["sortList"] = &ItemListBase::sortList;

	auto itemListBox = CEGUI.new_usertype<ItemListbox>("ItemListbox",
													   sol::base_classes, sol::bases<ScrolledItemListBase, ItemListBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	itemListBox["getSelectedCount"] = &ItemListbox::getSelectedCount;
	itemListBox["getLastSelectedItem"] = &ItemListbox::getLastSelectedItem;
	itemListBox["getFirstSelectedItem"] = &ItemListbox::getFirstSelectedItem;
	itemListBox["getNextSelectedItem"] = &ItemListbox::getNextSelectedItem;
	itemListBox["getNextSelectedItemAfter"] = &ItemListbox::getNextSelectedItemAfter;
	itemListBox["isMultiSelectEnabled"] = &ItemListbox::isMultiSelectEnabled;
	itemListBox["isItemSelected"] = &ItemListbox::isItemSelected;
	itemListBox["setMultiSelectEnabled"] = &ItemListbox::setMultiSelectEnabled;
	itemListBox["clearAllSelections"] = &ItemListbox::clearAllSelections;
	itemListBox["selectRange"] = &ItemListbox::selectRange;
	itemListBox["selectAllItems"] = &ItemListbox::selectAllItems;


	auto layoutContainer = CEGUI.new_usertype<LayoutContainer>("LayoutContainer",
															   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);

	layoutContainer["markNeedsLayouting"] = &LayoutContainer::markNeedsLayouting;
	layoutContainer["needsLayouting"] = &LayoutContainer::needsLayouting;
	layoutContainer["layout"] = &LayoutContainer::layout;
	layoutContainer["layoutIfNecessary"] = &LayoutContainer::layoutIfNecessary;

	auto listbox = CEGUI.new_usertype<Listbox>("Listbox",
											   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);

	listbox["getItemCount"] = &Listbox::getItemCount;
	listbox["getSelectedCount"] = &Listbox::getSelectedCount;
	listbox["getFirstSelectedItem"] = &Listbox::getFirstSelectedItem;
	listbox["getNextSelected"] = &Listbox::getNextSelected;
	listbox["getListboxItemFromIndex"] = &Listbox::getListboxItemFromIndex;
	listbox["getItemIndex"] = &Listbox::getItemIndex;
	listbox["findItemWithText"] = [](Listbox* self, const char* text, const ListboxItem* start_item) { return self->findItemWithText(text, start_item); };
	listbox["isSortEnabled"] = &Listbox::isSortEnabled;
	listbox["isMultiselectEnabled"] = &Listbox::isMultiselectEnabled;
	listbox["isItemSelected"] = &Listbox::isItemSelected;
	listbox["isListboxItemInList"] = &Listbox::isListboxItemInList;
	listbox["isVertScrollbarAlwaysShown"] = &Listbox::isVertScrollbarAlwaysShown;
	listbox["isHorzScrollbarAlwaysShown"] = &Listbox::isHorzScrollbarAlwaysShown;
	listbox["resetList"] = &Listbox::resetList;
	listbox["addItem"] = &Listbox::addItem;
	listbox["insertItem"] = &Listbox::insertItem;
	listbox["removeItem"] = &Listbox::removeItem;
	listbox["clearAllSelections"] = &Listbox::clearAllSelections;
	listbox["setSortingEnabled"] = &Listbox::setSortingEnabled;
	listbox["setMultiselectEnabled"] = &Listbox::setMultiselectEnabled;
	listbox["setShowVertScrollbar"] = &Listbox::setShowVertScrollbar;
	listbox["setShowHorzScrollbar"] = &Listbox::setShowHorzScrollbar;
	listbox["setItemSelectState"] = sol::overload(sol::resolve<ListboxItem*, bool>(&Listbox::setItemSelectState),
												  sol::resolve<size_t, bool>(&Listbox::setItemSelectState));
	listbox["handleUpdatedItemData"] = &Listbox::handleUpdatedItemData;
	listbox["ensureItemIsVisible"] = sol::overload(sol::resolve<void(
														   const ListboxItem*)>(&Listbox::ensureItemIsVisible),
												   sol::resolve<void(size_t)>(&Listbox::ensureItemIsVisible));
	listbox["getListRenderArea"] = &Listbox::getListRenderArea;
	listbox["getVertScrollbar"] = &Listbox::getVertScrollbar;
	listbox["getHorzScrollbar"] = &Listbox::getHorzScrollbar;
	listbox["getWidestItemWidth"] = &Listbox::getWidestItemWidth;
	listbox["getTotalItemsHeight"] = &Listbox::getTotalItemsHeight;
	listbox["getItemAtPoint"] = &Listbox::getItemAtPoint;

	auto listboxItem = CEGUI.new_usertype<ListboxItem>("ListboxItem");


	listboxItem["getText"] = string_getter(&ListboxItem::getText);
	listboxItem["getTooltipText"] = string_getter(&ListboxItem::getTooltipText);
	listboxItem["getID"] = &ListboxItem::getID;
	listboxItem["getUserData"] = &ListboxItem::getUserData;
	listboxItem["isSelected"] = &ListboxItem::isSelected;
	listboxItem["isDisabled"] = &ListboxItem::isDisabled;
	listboxItem["isAutoDeleted"] = &ListboxItem::isAutoDeleted;
	listboxItem["getOwnerWindow"] = &ListboxItem::getOwnerWindow;
	listboxItem["getSelectionColours"] = &ListboxItem::getSelectionColours;
	listboxItem["getSelectionBrushImage"] = &ListboxItem::getSelectionBrushImage;
	listboxItem["setText"] = string_setter(&ListboxItem::setText);
	listboxItem["setTooltipText"] = string_setter(&ListboxItem::setTooltipText);
	listboxItem["setID"] = &ListboxItem::setID;
	listboxItem["setUserData"] = &ListboxItem::setUserData;
	listboxItem["setSelected"] = &ListboxItem::setSelected;
	listboxItem["setAutoDeleted"] = &ListboxItem::setAutoDeleted;
	listboxItem["setSelectionColours"] = sol::overload(sol::resolve<void(
															   const ColourRect&)>(&ListboxItem::setSelectionColours),
													   sol::resolve<void(Colour, Colour, Colour, Colour)>(&ListboxItem::setSelectionColours),
													   sol::resolve<void(Colour)>(&ListboxItem::setSelectionColours));
	listboxItem["setSelectionBrushImage"] = sol::overload(sol::resolve<void(
																  const Image*)>(&ListboxItem::setSelectionBrushImage),
														  [](ListboxItem* self, const char* name) { self->setSelectionBrushImage(name); });
	listboxItem["getPixelSize"] = &ListboxItem::getPixelSize;

	auto listboxTextItem = CEGUI.new_usertype<ListboxTextItem>("ListboxTextItem",
															   sol::base_classes, sol::bases<ListboxItem>()
	);
	listboxTextItem["getFont"] = &ListboxTextItem::getFont;
	listboxTextItem["getTextColours"] = &ListboxTextItem::getTextColours;
	listboxTextItem["setFont"] = sol::overload(sol::resolve<void(Font*)>(&ListboxTextItem::setFont),
											   [](ListboxTextItem* self, const char* font) { self->setFont(font); });
	listboxTextItem["setTextColours"] = sol::overload(sol::resolve<void(
															  const ColourRect&)>(&ListboxTextItem::setTextColours),
													  sol::resolve<void(Colour, Colour, Colour, Colour)>(&ListboxTextItem::setTextColours),
													  sol::resolve<void(Colour)>(&ListboxTextItem::setTextColours));
	listboxTextItem["setTextParsingEnabled"] = &ListboxTextItem::setTextParsingEnabled;
	listboxTextItem["isTextParsingEnabled"] = &ListboxTextItem::isTextParsingEnabled;

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
	auto menuBase = CEGUI.new_usertype<MenuBase>("MenuBase",
												 sol::base_classes, sol::bases<ItemListBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	menuBase["getItemSpacing"] = &MenuBase::getItemSpacing;
	menuBase["isMultiplePopupsAllowed"] = &MenuBase::isMultiplePopupsAllowed;
	menuBase["getPopupMenuItem"] = &MenuBase::getPopupMenuItem;
	menuBase["setItemSpacing"] = &MenuBase::setItemSpacing;
	menuBase["changePopupMenuItem"] = &MenuBase::changePopupMenuItem;
	menuBase["setAllowMultiplePopups"] = &MenuBase::setAllowMultiplePopups;
	menuBase["getAutoCloseNestedPopups"] = &MenuBase::getAutoCloseNestedPopups;
	menuBase["getPopupMenuItem"] = &MenuBase::getPopupMenuItem;

	auto menuItem = CEGUI.new_usertype<MenuItem>("MenuItem",
												 sol::base_classes, sol::bases<ItemEntry, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	menuItem["isHovering"] = &MenuItem::isHovering;
	menuItem["isPushed"] = &MenuItem::isPushed;
	menuItem["getPopupMenu"] = &MenuItem::getPopupMenu;
	menuItem["setPopupMenu"] = &MenuItem::setPopupMenu;
	menuItem["openPopupMenu"] = sol::overload(&MenuItem::openPopupMenu, [](MenuItem* self) { self->openPopupMenu(); });
	menuItem["closePopupMenu"] = sol::overload(&MenuItem::closePopupMenu, [](MenuItem* self) { self->closePopupMenu(); });
	menuItem["togglePopupMenu"] = &MenuItem::togglePopupMenu;
	menuItem["hasAutoPopup"] = &MenuItem::hasAutoPopup;
	menuItem["getAutoPopupTimeout"] = &MenuItem::getAutoPopupTimeout;
	menuItem["setAutoPopupTimeout"] = &MenuItem::setAutoPopupTimeout;
	menuItem["getPopupOffset"] = &MenuItem::getPopupOffset;
	menuItem["setPopupOffset"] = &MenuItem::setPopupOffset;

	CEGUI.new_usertype<MCLGridRef>("MCLGridRef",
								   sol::constructors<MCLGridRef(unsigned int, unsigned int)>(),
								   "row", &MCLGridRef::row,
								   "column", &MCLGridRef::column
	);


	auto multiColumnList = CEGUI.new_usertype<MultiColumnList>("MultiColumnList",
															   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);

	multiColumnList["RowSingle"] = sol::var(MultiColumnList::RowSingle);
	multiColumnList["CellSingle"] = sol::var(MultiColumnList::CellSingle);
	multiColumnList["CellMultiple"] = sol::var(MultiColumnList::CellMultiple);
	multiColumnList["NominatedColumnSingle"] = sol::var(MultiColumnList::NominatedColumnSingle);
	multiColumnList["NominatedColumnMultiple"] = sol::var(MultiColumnList::NominatedColumnMultiple);
	multiColumnList["ColumnSingle"] = sol::var(MultiColumnList::ColumnSingle);
	multiColumnList["ColumnMultiple"] = sol::var(MultiColumnList::ColumnMultiple);
	multiColumnList["NominatedRowSingle"] = sol::var(MultiColumnList::NominatedRowSingle);
	multiColumnList["NominatedRowMultiple"] = sol::var(MultiColumnList::NominatedRowMultiple);
	multiColumnList["isUserSortControlEnabled"] = &MultiColumnList::isUserSortControlEnabled;
	multiColumnList["isUserColumnSizingEnabled"] = &MultiColumnList::isUserColumnSizingEnabled;
	multiColumnList["isUserColumnDraggingEnabled"] = &MultiColumnList::isUserColumnDraggingEnabled;
	multiColumnList["getColumnCount"] = &MultiColumnList::getColumnCount;
	multiColumnList["getRowCount"] = &MultiColumnList::getRowCount;
	multiColumnList["getSortColumn"] = &MultiColumnList::getSortColumn;
	multiColumnList["getColumnWithID"] = &MultiColumnList::getColumnWithID;
	multiColumnList["getColumnWithHeaderText"] = string_setter(&MultiColumnList::getColumnWithHeaderText);
	multiColumnList["getTotalColumnHeadersWidth"] = &MultiColumnList::getTotalColumnHeadersWidth;
	multiColumnList["getColumnHeaderWidth"] = &MultiColumnList::getColumnHeaderWidth;
	multiColumnList["getSortDirection"] = &MultiColumnList::getSortDirection;
	multiColumnList["getHeaderSegmentForColumn"] = &MultiColumnList::getHeaderSegmentForColumn;
	multiColumnList["getItemRowIndex"] = &MultiColumnList::getItemRowIndex;
	multiColumnList["getItemColumnIndex"] = &MultiColumnList::getItemColumnIndex;
	multiColumnList["getItemGridReference"] = &MultiColumnList::getItemGridReference;
	multiColumnList["getItemAtGridReference"] = &MultiColumnList::getItemAtGridReference;
	multiColumnList["isListboxItemInColumn"] = &MultiColumnList::isListboxItemInColumn;
	multiColumnList["isListboxItemInRow"] = &MultiColumnList::isListboxItemInRow;
	multiColumnList["isListboxItemInList"] = &MultiColumnList::isListboxItemInList;
//									multiColumnList["findColumnItemWithText"]= string_setter(&MultiColumnList::findColumnItemWithText);
//									multiColumnList["findRowItemWithText"]= string_setter(&MultiColumnList::findRowItemWithText);
//									multiColumnList["findListItemWithText"]= string_setter(&MultiColumnList::findListItemWithText);
	multiColumnList["getFirstSelectedItem"] = &MultiColumnList::getFirstSelectedItem;
	multiColumnList["getNextSelected"] = &MultiColumnList::getNextSelected;
	multiColumnList["getSelectedCount"] = &MultiColumnList::getSelectedCount;
	multiColumnList["isItemSelected"] = &MultiColumnList::isItemSelected;
	multiColumnList["getNominatedSelectionColumnID"] = &MultiColumnList::getNominatedSelectionColumnID;
	multiColumnList["getNominatedSelectionColumn"] = &MultiColumnList::getNominatedSelectionColumn;
	multiColumnList["getNominatedSelectionRow"] = &MultiColumnList::getNominatedSelectionRow;
	multiColumnList["getSelectionMode"] = &MultiColumnList::getSelectionMode;
	multiColumnList["isVertScrollbarAlwaysShown"] = &MultiColumnList::isVertScrollbarAlwaysShown;
	multiColumnList["isHorzScrollbarAlwaysShown"] = &MultiColumnList::isHorzScrollbarAlwaysShown;
	multiColumnList["getColumnID"] = &MultiColumnList::getColumnID;
	multiColumnList["getRowID"] = &MultiColumnList::getRowID;
	multiColumnList["getRowWithID"] = &MultiColumnList::getRowWithID;
	multiColumnList["getListRenderArea"] = &MultiColumnList::getListRenderArea;
	multiColumnList["getVertScrollbar"] = &MultiColumnList::getVertScrollbar;
	multiColumnList["getHorzScrollbar"] = &MultiColumnList::getHorzScrollbar;
	multiColumnList["getListHeader"] = &MultiColumnList::getListHeader;
	multiColumnList["getTotalRowsHeight"] = &MultiColumnList::getTotalRowsHeight;
	multiColumnList["getWidestColumnItemWidth"] = &MultiColumnList::getWidestColumnItemWidth;
	multiColumnList["getHighestRowItemHeight"] = &MultiColumnList::getHighestRowItemHeight;
	multiColumnList["resetList"] = &MultiColumnList::resetList;
	multiColumnList["addColumn"] = [](MultiColumnList* self, const char* text, unsigned int col_id, const UDim& width) { self->addColumn(text, col_id, width); };
	multiColumnList["insertColumn"] = [](MultiColumnList* self, const char* text, unsigned int col_id, const UDim& width, unsigned int position) { self->insertColumn(text, col_id, width, position); };
	multiColumnList["removeColumn"] = &MultiColumnList::removeColumn;
	multiColumnList["removeColumnWithID"] = &MultiColumnList::removeColumnWithID;
	multiColumnList["moveColumn"] = &MultiColumnList::moveColumn;
	multiColumnList["moveColumnWithID"] = &MultiColumnList::moveColumnWithID;
	multiColumnList["addRow"] = sol::overload([](MultiColumnList* self) { return self->addRow(); }, [](MultiColumnList* self, ListboxItem* item, unsigned int col_id) { return self->addRow(item, col_id); });
	multiColumnList["insertRow"] = sol::overload([](MultiColumnList* self, unsigned int row_idx) { return self->insertRow(row_idx); }, [](MultiColumnList* self, ListboxItem* item, unsigned int col_id, unsigned int row_idx) { return self->insertRow(item, col_id, row_idx); });
	multiColumnList["removeRow"] = &MultiColumnList::removeRow;
	multiColumnList["setItem"] = sol::overload(sol::resolve<ListboxItem*, const MCLGridRef&>(&MultiColumnList::setItem), sol::resolve<ListboxItem*, unsigned int, unsigned int>(&MultiColumnList::setItem));
	multiColumnList["setSelectionMode"] = &MultiColumnList::setSelectionMode;
	multiColumnList["setNominatedSelectionColumnID"] = &MultiColumnList::setNominatedSelectionColumnID;
	multiColumnList["setNominatedSelectionColumn"] = &MultiColumnList::setNominatedSelectionColumn;
	multiColumnList["setNominatedSelectionRow"] = &MultiColumnList::setNominatedSelectionRow;
	multiColumnList["setSortDirection"] = &MultiColumnList::setSortDirection;
	multiColumnList["setSortColumn"] = &MultiColumnList::setSortColumn;
	multiColumnList["setSortColumnByID"] = &MultiColumnList::setSortColumnByID;
	multiColumnList["setShowVertScrollbar"] = &MultiColumnList::setShowVertScrollbar;
	multiColumnList["setShowHorzScrollbar"] = &MultiColumnList::setShowHorzScrollbar;
	multiColumnList["clearAllSelections"] = &MultiColumnList::clearAllSelections;
	multiColumnList["setItemSelectState"] = sol::overload(sol::resolve<ListboxItem*, bool>(&MultiColumnList::setItemSelectState), sol::resolve<const MCLGridRef&, bool>(&MultiColumnList::setItemSelectState));
	multiColumnList["handleUpdatedItemData"] = &MultiColumnList::handleUpdatedItemData;
	multiColumnList["setColumnHeaderWidth"] = &MultiColumnList::setColumnHeaderWidth;
	multiColumnList["setUserSortControlEnabled"] = &MultiColumnList::setUserSortControlEnabled;
	multiColumnList["setUserColumnSizingEnabled"] = &MultiColumnList::setUserColumnSizingEnabled;
	multiColumnList["setUserColumnDraggingEnabled"] = &MultiColumnList::setUserColumnDraggingEnabled;
	multiColumnList["autoSizeColumnHeader"] = &MultiColumnList::autoSizeColumnHeader;
	multiColumnList["setRowID"] = &MultiColumnList::setRowID;
	multiColumnList["ensureRowIsVisible"] = &MultiColumnList::ensureRowIsVisible;
	multiColumnList["ensureColumnIsVisible"] = &MultiColumnList::ensureColumnIsVisible;
	multiColumnList["ensureItemRowIsVisible"] = &MultiColumnList::ensureItemRowIsVisible;
	multiColumnList["ensureItemColumnIsVisible"] = &MultiColumnList::ensureItemColumnIsVisible;
	multiColumnList["ensureItemIsVisible"] = sol::overload(sol::resolve<const ListboxItem*>(&MultiColumnList::ensureItemIsVisible), sol::resolve<const MCLGridRef&>(&MultiColumnList::ensureItemIsVisible));
	multiColumnList["setAutoSizeColumnUsesHeader"] = &MultiColumnList::setAutoSizeColumnUsesHeader;
	multiColumnList["getAutoSizeColumnUsesHeader"] = &MultiColumnList::getAutoSizeColumnUsesHeader;

	auto multiLineEditbox = CEGUI.new_usertype<MultiLineEditbox>("MultiLineEditbox",
																 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	multiLineEditbox["hasInputFocus"] = &MultiLineEditbox::hasInputFocus;
	multiLineEditbox["isReadOnly"] = &MultiLineEditbox::isReadOnly;
	multiLineEditbox["getCaretIndex"] = &MultiLineEditbox::getCaretIndex;
	multiLineEditbox["getSelectionStartIndex"] = &MultiLineEditbox::getSelectionStartIndex;
	multiLineEditbox["getSelectionEndIndex"] = &MultiLineEditbox::getSelectionEndIndex;
	multiLineEditbox["getSelectionLength"] = &MultiLineEditbox::getSelectionLength;
	multiLineEditbox["getMaxTextLength"] = &MultiLineEditbox::getMaxTextLength;
	multiLineEditbox["isWordWrapped"] = &MultiLineEditbox::isWordWrapped;
	multiLineEditbox["getVertScrollbar"] = &MultiLineEditbox::getVertScrollbar;
	multiLineEditbox["isVertScrollbarAlwaysShown"] = &MultiLineEditbox::isVertScrollbarAlwaysShown;
	multiLineEditbox["getHorzScrollbar"] = &MultiLineEditbox::getHorzScrollbar;
	multiLineEditbox["getTextRenderArea"] = &MultiLineEditbox::getTextRenderArea;
	multiLineEditbox["getLineNumberFromIndex"] = &MultiLineEditbox::getLineNumberFromIndex;
	multiLineEditbox["setReadOnly"] = &MultiLineEditbox::setReadOnly;
	multiLineEditbox["setCaretIndex"] = &MultiLineEditbox::setCaretIndex;
	multiLineEditbox["setSelection"] = &MultiLineEditbox::setSelection;
	multiLineEditbox["setMaxTextLength"] = &MultiLineEditbox::setMaxTextLength;
	multiLineEditbox["ensureCaretIsVisible"] = &MultiLineEditbox::ensureCaretIsVisible;
	multiLineEditbox["setWordWrapping"] = &MultiLineEditbox::setWordWrapping;
	multiLineEditbox["setShowVertScrollbar"] = &MultiLineEditbox::setShowVertScrollbar;
	multiLineEditbox["setSelectionBrushImage"] = &MultiLineEditbox::setSelectionBrushImage;
	multiLineEditbox["getSelectionBrushImage"] = &MultiLineEditbox::getSelectionBrushImage;


	auto popupMenu = CEGUI.new_usertype<PopupMenu>("PopupMenu",
												   sol::base_classes, sol::bases<MenuBase, ItemListBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	popupMenu["getFadeInTime"] = &PopupMenu::getFadeInTime;
	popupMenu["getFadeOutTime"] = &PopupMenu::getFadeOutTime;
	popupMenu["setFadeInTime"] = &PopupMenu::setFadeInTime;
	popupMenu["setFadeOutTime"] = &PopupMenu::setFadeOutTime;
	popupMenu["openPopupMenu"] = sol::overload(&PopupMenu::openPopupMenu, [](PopupMenu* self) { self->openPopupMenu(); });
	popupMenu["closePopupMenu"] = sol::overload(&PopupMenu::closePopupMenu, [](PopupMenu* self) { self->closePopupMenu(); });

	auto progressBar = CEGUI.new_usertype<ProgressBar>("ProgressBar",
													   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	progressBar["getProgress"] = &ProgressBar::getProgress;
	progressBar["getStepSize"] = &ProgressBar::getStepSize;
	progressBar["setProgress"] = &ProgressBar::setProgress;
	progressBar["setStepSize"] = &ProgressBar::setStepSize;
	progressBar["step"] = &ProgressBar::step;
	progressBar["adjustProgress"] = &ProgressBar::adjustProgress;

	auto radioButton = CEGUI.new_usertype<RadioButton>("RadioButton",
													   sol::base_classes, sol::bases<ToggleButton, ButtonBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	radioButton["getGroupID"] = &RadioButton::getGroupID;
	radioButton["setGroupID"] = &RadioButton::setGroupID;
	radioButton["getSelectedButtonInGroup"] = &RadioButton::getSelectedButtonInGroup;
	auto scrollablePane = CEGUI.new_usertype<ScrollablePane>("ScrollablePane",
															 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	scrollablePane["getContentPane"] = &ScrollablePane::getContentPane;
	scrollablePane["isVertScrollbarAlwaysShown"] = &ScrollablePane::isVertScrollbarAlwaysShown;
	scrollablePane["setShowVertScrollbar"] = &ScrollablePane::setShowVertScrollbar;
	scrollablePane["isHorzScrollbarAlwaysShown"] = &ScrollablePane::isHorzScrollbarAlwaysShown;
	scrollablePane["setShowHorzScrollbar"] = &ScrollablePane::setShowHorzScrollbar;
	scrollablePane["isContentPaneAutoSized"] = &ScrollablePane::isContentPaneAutoSized;
	scrollablePane["setContentPaneAutoSized"] = &ScrollablePane::setContentPaneAutoSized;
	scrollablePane["getContentPaneArea"] = &ScrollablePane::getContentPaneArea;
	scrollablePane["setContentPaneArea"] = &ScrollablePane::setContentPaneArea;
	scrollablePane["getHorizontalStepSize"] = &ScrollablePane::getHorizontalStepSize;
	scrollablePane["setHorizontalStepSize"] = &ScrollablePane::setHorizontalStepSize;
	scrollablePane["getHorizontalOverlapSize"] = &ScrollablePane::getHorizontalOverlapSize;
	scrollablePane["setHorizontalOverlapSize"] = &ScrollablePane::setHorizontalOverlapSize;
	scrollablePane["getHorizontalScrollPosition"] = &ScrollablePane::getHorizontalScrollPosition;
	scrollablePane["setHorizontalScrollPosition"] = &ScrollablePane::setHorizontalScrollPosition;
	scrollablePane["getVerticalStepSize"] = &ScrollablePane::getVerticalStepSize;
	scrollablePane["setVerticalStepSize"] = &ScrollablePane::setVerticalStepSize;
	scrollablePane["getVerticalOverlapSize"] = &ScrollablePane::getVerticalOverlapSize;
	scrollablePane["setVerticalOverlapSize"] = &ScrollablePane::setVerticalOverlapSize;
	scrollablePane["getVerticalScrollPosition"] = &ScrollablePane::getVerticalScrollPosition;
	scrollablePane["setVerticalScrollPosition"] = &ScrollablePane::setVerticalScrollPosition;
	scrollablePane["getViewableArea"] = &ScrollablePane::getViewableArea;
	scrollablePane["getVertScrollbar"] = &ScrollablePane::getVertScrollbar;
	scrollablePane["getHorzScrollbar"] = &ScrollablePane::getHorzScrollbar;
	auto scrollBar = CEGUI.new_usertype<Scrollbar>("Scrollbar",
												   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	scrollBar["getDocumentSize"] = &Scrollbar::getDocumentSize;
	scrollBar["getPageSize"] = &Scrollbar::getPageSize;
	scrollBar["getStepSize"] = &Scrollbar::getStepSize;
	scrollBar["getOverlapSize"] = &Scrollbar::getOverlapSize;
	scrollBar["getScrollPosition"] = &Scrollbar::getScrollPosition;
	scrollBar["getUnitIntervalScrollPosition"] = &Scrollbar::getUnitIntervalScrollPosition;
	scrollBar["isEndLockEnabled"] = &Scrollbar::isEndLockEnabled;
	scrollBar["setDocumentSize"] = &Scrollbar::setDocumentSize;
	scrollBar["setPageSize"] = &Scrollbar::setPageSize;
	scrollBar["setStepSize"] = &Scrollbar::setStepSize;
	scrollBar["setOverlapSize"] = &Scrollbar::setOverlapSize;
	scrollBar["setScrollPosition"] = &Scrollbar::setScrollPosition;
	scrollBar["setUnitIntervalScrollPosition"] = &Scrollbar::setUnitIntervalScrollPosition;
	scrollBar["setEndLockEnabled"] = &Scrollbar::setEndLockEnabled;
	scrollBar["scrollForwardsByStep"] = &Scrollbar::scrollForwardsByStep;
	scrollBar["scrollBackwardsByStep"] = &Scrollbar::scrollBackwardsByStep;
	scrollBar["scrollForwardsByPage"] = &Scrollbar::scrollForwardsByPage;
	scrollBar["scrollBackwardsByPage"] = &Scrollbar::scrollBackwardsByPage;
	auto scrolledContainer = CEGUI.new_usertype<ScrolledContainer>("ScrolledContainer",
																   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	scrolledContainer["isContentPaneAutoSized"] = &ScrolledContainer::isContentPaneAutoSized;
	scrolledContainer["setContentPaneAutoSized"] = &ScrolledContainer::setContentPaneAutoSized;
	scrolledContainer["getContentArea"] = &ScrolledContainer::getContentArea;
	scrolledContainer["setContentArea"] = &ScrolledContainer::setContentArea;
	scrolledContainer["getChildExtentsArea"] = &ScrolledContainer::getChildExtentsArea;
	auto scrolledItemListBase = CEGUI.new_usertype<ScrolledItemListBase>("ScrolledItemListBase",
																		 sol::base_classes, sol::bases<ItemListBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	scrolledItemListBase["isVertScrollbarAlwaysShown"] = &ScrolledItemListBase::isVertScrollbarAlwaysShown;
	scrolledItemListBase["isHorzScrollbarAlwaysShown"] = &ScrolledItemListBase::isHorzScrollbarAlwaysShown;
	scrolledItemListBase["getVertScrollbar"] = &ScrolledItemListBase::getVertScrollbar;
	scrolledItemListBase["getHorzScrollbar"] = &ScrolledItemListBase::getHorzScrollbar;
	scrolledItemListBase["setShowVertScrollbar"] = &ScrolledItemListBase::setShowVertScrollbar;
	scrolledItemListBase["setShowHorzScrollbar"] = &ScrolledItemListBase::setShowHorzScrollbar;

	auto sequentialLayoutContainer = CEGUI.new_usertype<SequentialLayoutContainer>("SequentialLayoutContainer",
																				   sol::base_classes, sol::bases<LayoutContainer, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	sequentialLayoutContainer["getPositionOfChild"] = sol::overload(
			sol::resolve<size_t(Window*) const>(&SequentialLayoutContainer::getPositionOfChild),
			[](SequentialLayoutContainer* self, const char* wnd) { return self->getPositionOfChild(wnd); });
	sequentialLayoutContainer["getChildAtPosition"] = &SequentialLayoutContainer::getChildAtPosition;
	sequentialLayoutContainer["swapChildPositions"] = &SequentialLayoutContainer::swapChildPositions;
	sequentialLayoutContainer["swapChildren"] = sol::overload(sol::resolve<void(Window*, Window*)>(&SequentialLayoutContainer::swapChildren),
															  [](SequentialLayoutContainer* self, const char* wnd1, Window* wnd2) { self->swapChildren(wnd1, wnd2); },
															  [](SequentialLayoutContainer* self, const char* wnd1, const char* wnd2) { self->swapChildren(wnd1, wnd2); },
															  [](SequentialLayoutContainer* self, Window* wnd1, const char* wnd2) { self->swapChildren(wnd1, wnd2); });
	sequentialLayoutContainer["moveChildToPosition"] = sol::overload(sol::resolve<void(Window*, size_t)>(&SequentialLayoutContainer::moveChildToPosition),
																	 [](SequentialLayoutContainer* self, const char* wnd, size_t position) { self->moveChildToPosition(wnd, position); });
	sequentialLayoutContainer["addChildToPosition"] = &SequentialLayoutContainer::addChildToPosition;
	sequentialLayoutContainer["removeChildFromPosition"] = &SequentialLayoutContainer::removeChildFromPosition;
	auto slider = CEGUI.new_usertype<Slider>("Slider",
											 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	slider["getCurrentValue"] = &Slider::getCurrentValue;
	slider["getMaxValue"] = &Slider::getMaxValue;
	slider["getClickStep"] = &Slider::getClickStep;
	slider["setMaxValue"] = &Slider::setMaxValue;
	slider["setCurrentValue"] = &Slider::setCurrentValue;
	slider["setClickStep"] = &Slider::setClickStep;
	auto spinner = CEGUI.new_usertype<Spinner>("Spinner",
											   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	spinner["FloatingPoint"] = sol::var(Spinner::FloatingPoint);
	spinner["Integer"] = sol::var(Spinner::Integer);
	spinner["Hexadecimal"] = sol::var(Spinner::Hexadecimal);
	spinner["Octal"] = sol::var(Spinner::Octal);
	spinner["getCurrentValue"] = &Spinner::getCurrentValue;
	spinner["getStepSize"] = &Spinner::getStepSize;
	spinner["getMaximumValue"] = &Spinner::getMaximumValue;
	spinner["getMinimumValue"] = &Spinner::getMinimumValue;
	spinner["getTextInputMode"] = &Spinner::getTextInputMode;
	spinner["setCurrentValue"] = &Spinner::setCurrentValue;
	spinner["setStepSize"] = &Spinner::setStepSize;
	spinner["setMaximumValue"] = &Spinner::setMaximumValue;
	spinner["setMinimumValue"] = &Spinner::setMinimumValue;
	spinner["setTextInputMode"] = &Spinner::setTextInputMode;
	auto tabButton = CEGUI.new_usertype<TabButton>("TabButton",
												   sol::base_classes, sol::bases<ButtonBase, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	tabButton["isSelected"] = &TabButton::isSelected;
	tabButton["setSelected"] = &TabButton::setSelected;
	tabButton["setTargetWindow"] = &TabButton::setTargetWindow;
	tabButton["getTargetWindow"] = &TabButton::getTargetWindow;

	auto tabControl = CEGUI.new_usertype<TabControl>("TabControl",
													 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	tabControl["getTabCount"] = &TabControl::getTabCount;
	tabControl["setSelectedTab"] = sol::overload([](TabControl* self, const char* name) { self->setSelectedTab(name); }, sol::resolve<void(unsigned int)>(&TabControl::setSelectedTab));
	tabControl["setSelectedTabAtIndex"] = &TabControl::setSelectedTabAtIndex;
	tabControl["getTabContentsAtIndex"] = &TabControl::getTabContentsAtIndex;
	tabControl["getTabContents"] = sol::overload([](TabControl* self, const char* name) { return self->getTabContents(name); }, sol::resolve<Window*(unsigned int) const>(&TabControl::getTabContents));
	tabControl["isTabContentsSelected"] = &TabControl::isTabContentsSelected;
	tabControl["getSelectedTabIndex"] = &TabControl::getSelectedTabIndex;
	tabControl["getTabHeight"] = &TabControl::getTabHeight;
	tabControl["getTabTextPadding"] = &TabControl::getTabTextPadding;
	tabControl["setTabHeight"] = &TabControl::setTabHeight;
	tabControl["setTabTextPadding"] = &TabControl::setTabTextPadding;
	tabControl["addTab"] = &TabControl::addTab;
	tabControl["removeTab"] = sol::overload([](TabControl* self, const char* name) { self->removeTab(name); }, sol::resolve<void(unsigned int)>(&TabControl::removeTab));

	auto thumb = CEGUI.new_usertype<Thumb>("Thumb",
										   sol::base_classes, sol::bases<PushButton, Window, NamedElement, Element, PropertySet, EventSet>()
	);
	thumb["isHotTracked"] = &Thumb::isHotTracked;
	thumb["isVertFree"] = &Thumb::isVertFree;
	thumb["isHorzFree"] = &Thumb::isHorzFree;
	thumb["setHotTracked"] = &Thumb::setHotTracked;
	thumb["setVertFree"] = &Thumb::setVertFree;
	thumb["setHorzFree"] = &Thumb::setHorzFree;
	thumb["setVertRange"] = sol::resolve<void(float
											  min, float
											  max)>(&Thumb::setVertRange);
	thumb["setHorzRange"] = sol::resolve<void(float
											  min, float
											  max)>(&Thumb::setHorzRange);
	thumb["getVertRange"] = [](Thumb* self) {
		auto range = self->getVertRange();
		return std::tuple(range.first, range.second);
	};
	thumb["getHorzRange"] = [](Thumb* self) {
		auto range = self->getHorzRange();
		return std::tuple(range.first, range.second);
	};
	auto titlebar = CEGUI.new_usertype<Titlebar>("Titlebar",
												 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	titlebar["isDraggingEnabled"] = &Titlebar::isDraggingEnabled;
	titlebar["setDraggingEnabled"] = &Titlebar::setDraggingEnabled;
	auto tooltip = CEGUI.new_usertype<Tooltip>("Tooltip",
											   sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	tooltip["setTargetWindow"] = &Tooltip::setTargetWindow;
	tooltip["getTargetWindow"] = &Tooltip::getTargetWindow;
	tooltip["resetTimer"] = &Tooltip::resetTimer;
	tooltip["setHoverTime"] = &Tooltip::setHoverTime;
	tooltip["getHoverTime"] = &Tooltip::getHoverTime;
	tooltip["setDisplayTime"] = &Tooltip::setDisplayTime;
	tooltip["getDisplayTime"] = &Tooltip::getDisplayTime;
	tooltip["positionSelf"] = &Tooltip::positionSelf;
	tooltip["sizeSelf"] = &Tooltip::sizeSelf;
	tooltip["getTextSize"] = &Tooltip::getTextSize;
	auto tree = CEGUI.new_usertype<Tree>("Tree",
										 sol::base_classes, sol::bases<Window, NamedElement, Element, PropertySet, EventSet>()
	);
	tree["getItemCount"] = &Tree::getItemCount;
	tree["getSelectedCount"] = &Tree::getSelectedCount;
	tree["getFirstSelectedItem"] = &Tree::getFirstSelectedItem;
	tree["getLastSelectedItem"] = &Tree::getLastSelectedItem;
	tree["isSortEnabled"] = &Tree::isSortEnabled;
	tree["isMultiselectEnabled"] = &Tree::isMultiselectEnabled;
	tree["isItemTooltipsEnabled"] = &Tree::isItemTooltipsEnabled;
	tree["findFirstItemWithText"] = [](Tree* self, const char* text) { return self->findFirstItemWithText(text); };
	tree["findNextItemWithText"] = [](Tree* self, const char* text, const TreeItem* start_item) { return self->findNextItemWithText(text, start_item); };
	tree["findFirstItemWithID"] = &Tree::findFirstItemWithID;
	tree["findNextItemWithID"] = &Tree::findNextItemWithID;
	tree["isTreeItemInList"] = &Tree::isTreeItemInList;
	tree["resetList"] = &Tree::resetList;
	tree["addItem"] = &Tree::addItem;
	tree["insertItem"] = &Tree::insertItem;
	tree["removeItem"] = &Tree::removeItem;
	tree["clearAllSelections"] = &Tree::clearAllSelections;
	tree["setSortingEnabled"] = &Tree::setSortingEnabled;
	tree["setMultiselectEnabled"] = &Tree::setMultiselectEnabled;
	tree["setItemSelectState"] = sol::overload(sol::resolve<void(TreeItem*, bool)>(&Tree::setItemSelectState),
											   sol::resolve<void(size_t, bool)>(&Tree::setItemSelectState));
	tree["ensureItemIsVisible"] = &Tree::ensureItemIsVisible;
	auto treeItem = CEGUI.new_usertype<TreeItem>("TreeItem");

	treeItem["getFont"] = &TreeItem::getFont;
	treeItem["getTextColours"] = &TreeItem::getTextColours;
	treeItem["setFont"] = sol::overload(sol::resolve<void(
												const Font*)>(&TreeItem::setFont),
										[](TreeItem* self, const char* font_name) { self->setFont(font_name); });
	treeItem["setTextColours"] = sol::overload(sol::resolve<void(
													   const ColourRect&)>(&TreeItem::setTextColours),
											   sol::resolve<void(Colour, Colour, Colour, Colour)>(&TreeItem::setTextColours),
											   sol::resolve<void(Colour)>(&TreeItem::setTextColours));
	treeItem["getText"] = [](TreeItem* self) { return std::string(self->getText().c_str()); };
	treeItem["getTooltipText"] = [](TreeItem* self) { return std::string(self->getTooltipText().c_str()); };
	treeItem["getID"] = &TreeItem::getID;
	treeItem["getUserData"] = &TreeItem::getUserData;
	treeItem["isSelected"] = &TreeItem::isSelected;
	treeItem["isDisabled"] = &TreeItem::isDisabled;
	treeItem["isAutoDeleted"] = &TreeItem::isAutoDeleted;
	treeItem["getOwnerWindow"] = &TreeItem::getOwnerWindow;
	treeItem["getSelectionColours"] = &TreeItem::getSelectionColours;
	treeItem["getSelectionBrushImage"] = &TreeItem::getSelectionBrushImage;
	treeItem["setText"] = string_setter(&TreeItem::setText);
	treeItem["setTooltipText"] = string_setter(&TreeItem::setTooltipText);
	treeItem["setID"] = &TreeItem::setID;
	treeItem["setUserData"] = &TreeItem::setUserData;
	treeItem["setSelected"] = &TreeItem::setSelected;
	treeItem["setDisabled"] = &TreeItem::setDisabled;
	treeItem["setAutoDeleted"] = &TreeItem::setAutoDeleted;
	treeItem["setOwnerWindow"] = &TreeItem::setOwnerWindow;
	treeItem["setSelectionColours"] = sol::overload(sol::resolve<void(
															const ColourRect&)>(&TreeItem::setSelectionColours),
													sol::resolve<void(Colour, Colour, Colour, Colour)>(&TreeItem::setSelectionColours),
													sol::resolve<void(Colour)>(&TreeItem::setSelectionColours));
	treeItem["setSelectionBrushImage"] = sol::overload(sol::resolve<const Image*>(&TreeItem::setSelectionBrushImage), [](TreeItem* self, const char* name) { self->setSelectionBrushImage(name); });
	treeItem["setButtonLocation"] = &TreeItem::setButtonLocation;
	treeItem["getButtonLocation"] = &TreeItem::getButtonLocation;
	treeItem["getIsOpen"] = &TreeItem::getIsOpen;
	treeItem["toggleIsOpen"] = &TreeItem::toggleIsOpen;
	treeItem["getTreeItemFromIndex"] = &TreeItem::getTreeItemFromIndex;
	treeItem["getItemCount"] = &TreeItem::getItemCount;
	treeItem["addItem"] = &TreeItem::addItem;
	treeItem["removeItem"] = &TreeItem::removeItem;
	treeItem["setIcon"] = &TreeItem::setIcon;
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


	auto font = CEGUI.new_usertype<Font>("Font",
										 sol::base_classes, sol::bases<PropertySet>());
	font["isCodepointAvailable"] = &Font::isCodepointAvailable;
	font["getTextExtent"] = [](Font* self, const char* text) { return self->getTextExtent(text); };
	font["getLineSpacing"] = [](Font* self) { return self->getLineSpacing(); };
	font["getFontHeight"] = [](Font* self) { return self->getFontHeight(); };


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
