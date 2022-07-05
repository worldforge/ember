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
#include "LuaFunctor.h"

#include <CEGUI/CEGUI.h>

using namespace Ember::Cegui;
using namespace CEGUI;

void registerBindingsCEGUI(sol::state_view& lua) {
	auto Ember = lua["Ember"].get_or_create<sol::table>();
	auto Cegui = Ember["Cegui"].get_or_create<sol::table>();
	registerLua<Helper>(Cegui);

	auto CEGUI = lua["CEGUI"].get_or_create<sol::table>();

	registerLua<EventSet>(CEGUI);
	registerLua<PropertySet>(CEGUI);
	registerLua<Element>(CEGUI);
	registerLua<Window>(CEGUI);
	registerLua<ButtonBase>(CEGUI);
	registerLua<System>(CEGUI);
	registerLua<Combobox>(CEGUI);
	registerLua<DragContainer>(CEGUI);
	registerLua<LayoutContainer>(CEGUI);
	registerLua<Editbox>(CEGUI);
	registerLua<FrameWindow>(CEGUI);
	registerLua<GroupBox>(CEGUI);
	registerLua<ItemListBase>(CEGUI);
	registerLua<Listbox>(CEGUI);
	registerLua<MenuBase>(CEGUI);
	registerLua<MultiColumnList>(CEGUI);
	registerLua<MultiLineEditbox>(CEGUI);
	registerLua<ProgressBar>(CEGUI);
	registerLua<ScrollablePane>(CEGUI);
	registerLua<Scrollbar>(CEGUI);
	registerLua<ScrolledContainer>(CEGUI);
	registerLua<Slider>(CEGUI);
	registerLua<Spinner>(CEGUI);
	registerLua<TabControl>(CEGUI);
	registerLua<Thumb>(CEGUI);
	registerLua<Titlebar>(CEGUI);
	registerLua<Tooltip>(CEGUI);
	registerLua<Tree>(CEGUI);
	registerLua<ImageManager>(CEGUI);
	registerLua<MouseCursor>(CEGUI);

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
