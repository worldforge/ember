//
// C++ Implementation: AdapterFactory
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "AdapterFactory.h"
#include "NumberAdapter.h"
#include "SizeAdapter.h"
#include "MapAdapter.h"
#include "ListAdapter.h"
#include "PositionAdapter.h"
#include "Position2DAdapter.h"
#include "OrientationAdapter.h"
#include "StaticAdapter.h"
#include "AreaAdapter.h"
#include "PolygonAdapter.h"
#include "TerrainModAdapter.h"
#include "ScaleAdapter.h"
#include "EntityRefAdapter.h"
#include "components/ogre/GUIManager.h"

using namespace CEGUI;

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

namespace Atlas {

unsigned long AdapterFactory::msAutoGenId = 0;

AdapterFactory::AdapterFactory(std::string prefix) :
		mPrefix(std::move(prefix)) {
}

AdapterFactory::~AdapterFactory() = default;

template<typename TAdapter>
TAdapter* AdapterFactory::createAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity) {
	if (!container) {
		S_LOG_FAILURE("No container sent to adapter factory.");
		return nullptr;
	}
	if (!verifyCorrectType<TAdapter>(element)) {
		S_LOG_FAILURE("Type is not correct for adapter.");
		return nullptr;
	}

	try {
		return loadWindowIntoAdapter<TAdapter>(container, adapterPrefix, element, entity);
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when loading adapter." << ex);
		return nullptr;
	}
}

template<>
bool AdapterFactory::verifyCorrectType<StringAdapter>(const ::Atlas::Message::Element& element) {
	return element.isString();
}

template<>
bool AdapterFactory::verifyCorrectType<NumberAdapter>(const ::Atlas::Message::Element& element) {
	return element.isNum();
}

template<>
bool AdapterFactory::verifyCorrectType<SizeAdapter>(const ::Atlas::Message::Element& element) {
	return element.isList();
}

template<>
bool AdapterFactory::verifyCorrectType<ScaleAdapter>(const ::Atlas::Message::Element& element) {
	return element.isList();
}

template<>
bool AdapterFactory::verifyCorrectType<OrientationAdapter>(const ::Atlas::Message::Element& element) {
	return element.isList();
}

template<>
bool AdapterFactory::verifyCorrectType<PositionAdapter>(const ::Atlas::Message::Element& element) {
	return element.isList();
}

template<>
bool AdapterFactory::verifyCorrectType<Position2DAdapter>(const ::Atlas::Message::Element& element) {
	return element.isList();
}

template<>
bool AdapterFactory::verifyCorrectType<ListAdapter>(const ::Atlas::Message::Element& element) {
	return element.isList();
}

template<>
bool AdapterFactory::verifyCorrectType<MapAdapter>(const ::Atlas::Message::Element& element) {
	return element.isMap();
}

template<>
bool AdapterFactory::verifyCorrectType<StaticAdapter>(const ::Atlas::Message::Element& element) {
	return true;
}

template<>
bool AdapterFactory::verifyCorrectType<AreaAdapter>(const ::Atlas::Message::Element& element) {
	return element.isMap();
}

template<>
bool AdapterFactory::verifyCorrectType<PolygonAdapter>(const ::Atlas::Message::Element& element) {
	return element.isMap();
}

template<>
bool AdapterFactory::verifyCorrectType<TerrainModAdapter>(const ::Atlas::Message::Element& element) {
	return element.isMap();
}

template<>
bool AdapterFactory::verifyCorrectType<EntityRefAdapter>(const ::Atlas::Message::Element& element) {
	return element.isMap();
}

template<>
StringAdapter* AdapterFactory::loadWindowIntoAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity) {
	loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/StringAdapter.layout");
	Combobox* stringWindow = static_cast<Combobox*> (container->getChildRecursive("String"));
	return new StringAdapter(element, stringWindow);
}

template<>
NumberAdapter* AdapterFactory::loadWindowIntoAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity) {
	loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/NumberAdapter.layout");
	Combobox* numberWindow = static_cast<Combobox*> (container->getChildRecursive("Number"));
	return new NumberAdapter(element, numberWindow);
}

template<>
SizeAdapter* AdapterFactory::loadWindowIntoAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity) {
	loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/SizeAdapter.layout");
	Window* lowerX = container->getChildRecursive("lowerX");
	Window* lowerY = container->getChildRecursive("lowerY");
	Window* lowerZ = container->getChildRecursive("lowerZ");
	Window* upperX = container->getChildRecursive("upperX");
	Window* upperY = container->getChildRecursive("upperY");
	Window* upperZ = container->getChildRecursive("upperZ");
	Slider* scaler = static_cast<Slider*> (container->getChildRecursive("scaler"));
	Window* info = container->getChildRecursive("info");
	ToggleButton* editable = static_cast<ToggleButton*>(container->getChildRecursive("editable"));
	return new SizeAdapter(element, {lowerX, lowerY, lowerZ, upperX, upperY, upperZ, scaler, info, editable});
}

template<>
ScaleAdapter* AdapterFactory::loadWindowIntoAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity) {
	loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/ScaleAdapter.layout");
	Window* x = container->getChildRecursive("x");
	Window* y = container->getChildRecursive("y");
	Window* z = container->getChildRecursive("z");
	ToggleButton* uniformToggle = dynamic_cast<ToggleButton*>(container->getChildRecursive("uniformToggle"));
	Slider* scaler = dynamic_cast<Slider*>(container->getChildRecursive("scaler"));

	return new ScaleAdapter(element, {x, y, z, uniformToggle, scaler});
}

template<>
OrientationAdapter* AdapterFactory::loadWindowIntoAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity) {
	loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/OrientationAdapter.layout");
	Window* x = container->getChildRecursive("x");
	Window* y = container->getChildRecursive("y");
	Window* z = container->getChildRecursive("z");
	Window* scalar = container->getChildRecursive("scalar");
	return new OrientationAdapter(element, x, y, z, scalar);
}

template<>
PositionAdapter* AdapterFactory::loadWindowIntoAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity) {
	loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/PositionAdapter.layout");
	Window* x = container->getChildRecursive("x");
	Window* y = container->getChildRecursive("y");
	Window* z = container->getChildRecursive("z");
	PushButton* moveButton = static_cast<PushButton*> (container->getChildRecursive("moveButton"));
	return new PositionAdapter(element, x, y, z, moveButton);
}

template<>
Position2DAdapter* AdapterFactory::loadWindowIntoAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity) {
	loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/Position2DAdapter.layout");
	Window* x = container->getChildRecursive("x");
	Window* y = container->getChildRecursive("y");
	return new Position2DAdapter(element, x, y);
}

template<>
MapAdapter* AdapterFactory::loadWindowIntoAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity) {
//	Window* window = loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/MapAdapter.layout");
	return new MapAdapter(element, nullptr);
}

template<>
ListAdapter* AdapterFactory::loadWindowIntoAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity) {
//	Window* window = loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/ListAdapter.layout");
	return new ListAdapter(element, nullptr);
}

template<>
StaticAdapter* AdapterFactory::loadWindowIntoAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity) {
	Window* window = loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/StaticAdapter.layout");
	return new StaticAdapter(element, window);
}

template<>
AreaAdapter* AdapterFactory::loadWindowIntoAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity) {
	loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/AreaAdapter.layout");
	PushButton* button = static_cast<PushButton*> (container->getChildRecursive("showButton"));
	Combobox* layerWindow = static_cast<Combobox*> (container->getChildRecursive("Layer"));
	return new AreaAdapter(element, button, layerWindow, entity);
}

template<>
PolygonAdapter* AdapterFactory::loadWindowIntoAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity) {
	loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/PolygonAdapter.layout");
	PushButton* button = static_cast<PushButton*> (container->getChildRecursive("showButton"));
	return new PolygonAdapter(element, button, entity);
}

template<>
TerrainModAdapter* AdapterFactory::loadWindowIntoAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity) {
	loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/TerrainModAdapter.layout");
	PushButton* button = static_cast<PushButton*> (container->getChildRecursive("showButton"));
	Combobox* posType = static_cast<Combobox*> (container->getChildRecursive("posType"));
	Combobox* modType = static_cast<Combobox*> (container->getChildRecursive("modType"));
	Editbox* heightEditbox = static_cast<Editbox*> (container->getChildRecursive("height"));
	return new TerrainModAdapter(element, button, entity, posType, modType, heightEditbox);
}

template<>
EntityRefAdapter* AdapterFactory::loadWindowIntoAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity) {
	loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/EntityRefAdapter.layout");
	Editbox* text = static_cast<Editbox*> (container->getChildRecursive("EntityRef"));
	return new EntityRefAdapter(element, {text});
}

StringAdapter* AdapterFactory::createStringAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element) {
	return createAdapter<StringAdapter>(container, adapterPrefix, element);
}

NumberAdapter* AdapterFactory::createNumberAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element) {
	return createAdapter<NumberAdapter>(container, adapterPrefix, element);
}

SizeAdapter* AdapterFactory::createSizeAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element) {
	return createAdapter<SizeAdapter>(container, adapterPrefix, element);
}

ScaleAdapter* AdapterFactory::createScaleAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element) {
	return createAdapter<ScaleAdapter>(container, adapterPrefix, element);
}

OrientationAdapter* AdapterFactory::createOrientationAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element) {
	return createAdapter<OrientationAdapter>(container, adapterPrefix, element);
}

PositionAdapter* AdapterFactory::createPositionAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element) {
	return createAdapter<PositionAdapter>(container, adapterPrefix, element);
}

Position2DAdapter* AdapterFactory::createPosition2DAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element) {
	return createAdapter<Position2DAdapter>(container, adapterPrefix, element);
}

MapAdapter* AdapterFactory::createMapAdapter(CEGUI::Window* container, const std::string& adapterPrefix, Eris::Entity* entity) {
	return createMapAdapter(container, adapterPrefix, entity->getProperties());
}

MapAdapter* AdapterFactory::createMapAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const std::map<std::string, ::Atlas::Message::Element>& attributes) {
	return createMapAdapter(container, adapterPrefix, ::Atlas::Message::Element(attributes));
}

MapAdapter* AdapterFactory::createMapAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element) {
	return createAdapter<MapAdapter>(container, adapterPrefix, element);
}

ListAdapter* AdapterFactory::createListAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element) {
	return createAdapter<ListAdapter>(container, adapterPrefix, element);
}

StaticAdapter* AdapterFactory::createStaticAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element) {
	return createAdapter<StaticAdapter>(container, adapterPrefix, element);
}

AreaAdapter* AdapterFactory::createAreaAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity) {
	return createAdapter<AreaAdapter>(container, adapterPrefix, element, entity);
}

PolygonAdapter* AdapterFactory::createPolygonAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity) {
	return createAdapter<PolygonAdapter>(container, adapterPrefix, element, entity);
}

TerrainModAdapter* AdapterFactory::createTerrainModAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity) {
	return createAdapter<TerrainModAdapter>(container, adapterPrefix, element, entity);
}

EntityRefAdapter* AdapterFactory::createEntityRefAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element) {
	return createAdapter<EntityRefAdapter>(container, adapterPrefix, element);
}

AdapterBase* AdapterFactory::createAdapterByType(const std::string& type, CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity) {
	::Atlas::Message::Element newElement(element);
	if (type == "string") {
		if (newElement.isNone()) {
			newElement = "";
		}
		return createStringAdapter(container, adapterPrefix, newElement);
	} else if (type == "number") {
		if (newElement.isNone()) {
			newElement = 0.0;
		}
		return createNumberAdapter(container, adapterPrefix, newElement);
	} else if (type == "size") {
		if (newElement.isNone()) {
			newElement = ::Atlas::Message::ListType();
		}
		return createSizeAdapter(container, adapterPrefix, newElement);
	} else if (type == "scale") {
		if (newElement.isNone()) {
			newElement = ::Atlas::Message::ListType();
		}
		return createScaleAdapter(container, adapterPrefix, newElement);
	} else if (type == "position") {
		if (newElement.isNone()) {
			::Atlas::Message::ListType list;
			list.push_back(::Atlas::Message::Element(0.0f));
			list.push_back(::Atlas::Message::Element(0.0f));
			list.push_back(::Atlas::Message::Element(0.0f));
			newElement = list;
		}
		return createPositionAdapter(container, adapterPrefix, newElement);
	} else if (type == "position2d") {
		if (newElement.isNone()) {
			::Atlas::Message::ListType list;
			list.push_back(::Atlas::Message::Element(0.0f));
			list.push_back(::Atlas::Message::Element(0.0f));
			newElement = list;
		}
		return createPosition2DAdapter(container, adapterPrefix, newElement);
	} else if (type == "map") {
		if (newElement.isNone()) {
			newElement = ::Atlas::Message::MapType();
		}
		return createNumberAdapter(container, adapterPrefix, newElement);
	} else if (type == "list") {
		if (newElement.isNone()) {
			newElement = ::Atlas::Message::ListType();
		}
		return createListAdapter(container, adapterPrefix, newElement);
	} else if (type == "orientation") {
		if (newElement.isNone()) {
			newElement = ::Atlas::Message::ListType();
		}
		return createNumberAdapter(container, adapterPrefix, newElement);
	} else if (type == "static") {
		if (newElement.isNone()) {
			newElement = "";
		}
		return createStaticAdapter(container, adapterPrefix, newElement);
	} else if (type == "area") {
		if (newElement.isNone()) {
			newElement = ::Atlas::Message::MapType();
		}
		return createAreaAdapter(container, adapterPrefix, newElement, entity);
	} else if (type == "polygon") {
		if (newElement.isNone()) {
			newElement = ::Atlas::Message::MapType();
		}
		return createPolygonAdapter(container, adapterPrefix, newElement, entity);
	} else {
		return nullptr;
	}
}

CEGUI::Window* AdapterFactory::loadLayoutIntoContainer(CEGUI::Window* container, const std::string& adapterPrefix, const std::string& layoutfile) {
	auto prototypeI = mAdapterPrototypes.find(layoutfile);

	CEGUI::Window* window = nullptr;
	if (prototypeI != mAdapterPrototypes.end()) {
		window = prototypeI->second->clone(true);
	} else {
		std::string finalFileName(GUIManager::getSingleton().getLayoutDir() + layoutfile);
		CEGUI::WindowManager& windowManager = CEGUI::WindowManager::getSingleton();
		auto windowNew = windowManager.loadLayoutFromFile(finalFileName);
		if (windowNew) {
			window = windowNew->clone(true);
			mAdapterPrototypes.emplace(layoutfile, UniqueWindowPtr<CEGUI::Window>(windowNew));
		}
	}
	if (!window) {
		S_LOG_FAILURE("Failed to create a new window from the layout in " << layoutfile << ".");
		return nullptr;
	}
	if (container) {
		container->addChild(window);
		container->setHeight(window->getHeight());
		container->setWidth(window->getWidth());
	}
	return window;

}

}

}

}

}
}
