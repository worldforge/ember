//
// C++ Implementation: AdapterFactory
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#include "AdapterFactory.h"
#include "StringAdapter.h"
#include "NumberAdapter.h"
#include "SizeAdapter.h"
#include "MapAdapter.h"
#include "ListAdapter.h"
#include "PositionAdapter.h"
#include "Position2DAdapter.h"
#include "OrientationAdapter.h"
#include "StaticAdapter.h"
#include <CEGUI.h>
#include "components/ogre/GUIManager.h"
#include "services/logging/LoggingService.h"
#include <Eris/Entity.h>

using namespace CEGUI;

namespace EmberOgre {

namespace Gui {

namespace Adapters {

namespace Atlas {

unsigned long AdapterFactory::msAutoGenId = 0;

AdapterFactory::AdapterFactory(const std::string prefix)
: mPrefix(prefix)
{
}


AdapterFactory::~AdapterFactory()
{
}

StringAdapter* AdapterFactory::createStringAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element)
{
	if (!container) {
		S_LOG_FAILURE("No container sent to AdapterFactory::createStringAdapter.");
		return 0;
	}
	
	if (!element.isString()) {
		S_LOG_FAILURE("Incorrect element type.");
		return 0;
	}
	
	try {
		loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/StringAdapter.layout");
		WindowManager& windowMgr = WindowManager::getSingleton();
		Combobox* stringWindow = static_cast<Combobox*>(windowMgr.getWindow(mCurrentPrefix + "String"));
		StringAdapter* adapter = new StringAdapter(element, stringWindow);
		
		return adapter;
	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("Error when creating StringAdapter. " << ex.getMessage ().c_str());
		return 0;
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when loading StringAdapter. " << ex.what());
		return 0;
	}

}

NumberAdapter* AdapterFactory::createNumberAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element)
{
	if (!container) {
		S_LOG_FAILURE("No container sent to AdapterFactory::createNumberAdapter.");
		return 0;
	}
	
	if (!element.isNum()) {
		S_LOG_FAILURE("Incorrect element type.");
		return 0;
	}
	
	try {
		
		loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/NumberAdapter.layout");
		WindowManager& windowMgr = WindowManager::getSingleton();
		Combobox* numberWindow = static_cast<Combobox*>(windowMgr.getWindow(mCurrentPrefix + "Number"));
		NumberAdapter* adapter = new NumberAdapter(element, numberWindow);
		
		return adapter;
	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("Error when creating NumberAdapter. " << ex.getMessage ().c_str());
		return 0;
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when loading NumberAdapter. " << ex.what());
		return 0;
	}

}

SizeAdapter* AdapterFactory::createSizeAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element)
{
	if (!container) {
		S_LOG_FAILURE("No container sent to AdapterFactory::createSizeAdapter.");
		return 0;
	}
	
	if (!element.isList()) {
		S_LOG_FAILURE("Incorrect element type.");
		return 0;
	}
	
	try {
		loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/SizeAdapter.layout");
		WindowManager& windowMgr = WindowManager::getSingleton();
		Window* lowerX = windowMgr.getWindow(mCurrentPrefix + "lowerX");
		Window* lowerY = windowMgr.getWindow(mCurrentPrefix + "lowerY");
		Window* lowerZ = windowMgr.getWindow(mCurrentPrefix + "lowerZ");
		Window* upperX = windowMgr.getWindow(mCurrentPrefix + "upperX");
		Window* upperY = windowMgr.getWindow(mCurrentPrefix + "upperY");
		Window* upperZ = windowMgr.getWindow(mCurrentPrefix + "upperZ");
		Slider* scaler = static_cast<Slider*>(windowMgr.getWindow(mCurrentPrefix + "scaler"));
		Window* info = windowMgr.getWindow(mCurrentPrefix + "info");
		SizeAdapter* adapter = new SizeAdapter(element, lowerX, lowerY, lowerZ, upperX, upperY, upperZ, scaler, info);
		
		return adapter;
	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("Error when creating SizeAdapter. " << ex.getMessage ().c_str());
		return 0;
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when loading SizeAdapter. " << ex.what());
		return 0;
	}
}

OrientationAdapter* AdapterFactory::createOrientationAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element)
{
	if (!container) {
		S_LOG_FAILURE("No container sent to AdapterFactory::createOrientationAdapter.");
		return 0;
	}
	
	if (!element.isList()) {
		S_LOG_FAILURE("Incorrect element type.");
		return 0;
	}
	
	try {
		loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/OrientationAdapter.layout");
		WindowManager& windowMgr = WindowManager::getSingleton();
		Window* x = windowMgr.getWindow(mCurrentPrefix + "x");
		Window* y = windowMgr.getWindow(mCurrentPrefix + "y");
		Window* z = windowMgr.getWindow(mCurrentPrefix + "z");
		Window* scalar = windowMgr.getWindow(mCurrentPrefix + "scalar");
		OrientationAdapter* adapter = new OrientationAdapter(element, x, y, z, scalar);
		
		return adapter;
	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("Error when creating OrientationAdapter. " << ex.getMessage ().c_str());
		return 0;
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when loading OrientationAdapter. " << ex.what());
		return 0;
	}
}

PositionAdapter* AdapterFactory::createPositionAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element)
{
	if (!container) {
		S_LOG_FAILURE("No container sent to AdapterFactory::createPositionAdapter.");
		return 0;
	}
	
	if (!element.isList()) {
		S_LOG_FAILURE("Incorrect element type.");
		return 0;
	}
	
	try {
		loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/PositionAdapter.layout");
		WindowManager& windowMgr = WindowManager::getSingleton();
		Window* x = windowMgr.getWindow(mCurrentPrefix + "x");
		Window* y = windowMgr.getWindow(mCurrentPrefix + "y");
		Window* z = windowMgr.getWindow(mCurrentPrefix + "z");
		PositionAdapter* adapter = new PositionAdapter(element, x, y, z);
		
		return adapter;
	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("Error when creating PositionAdapter. " << ex.getMessage ().c_str());
		return 0;
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when loading PositionAdapter. " << ex.what());
		return 0;
	}
}

Position2DAdapter* AdapterFactory::createPosition2DAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element)
{
	if (!container) {
		S_LOG_FAILURE("No container sent to AdapterFactory::createPosition2DAdapter.");
		return 0;
	}
	
	if (!element.isList()) {
		S_LOG_FAILURE("Incorrect element type.");
		return 0;
	}
	
	try {
		loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/Position2DAdapter.layout");
		WindowManager& windowMgr = WindowManager::getSingleton();
		Window* x = windowMgr.getWindow(mCurrentPrefix + "x");
		Window* y = windowMgr.getWindow(mCurrentPrefix + "y");
		Position2DAdapter* adapter = new Position2DAdapter(element, x, y);
		
		return adapter;
	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("Error when creating Position2DAdapter. " << ex.getMessage ().c_str());
		return 0;
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when loading Position2DAdapter. " << ex.what());
		return 0;
	}
}

MapAdapter* AdapterFactory::createMapAdapter(CEGUI::Window* container, const std::string& adapterPrefix, Eris::Entity* entity)
{
	return createMapAdapter(container, adapterPrefix, entity->getAttributes());
}

MapAdapter* AdapterFactory::createMapAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const std::map<std::string, ::Atlas::Message::Element> attributes)
{
	return createMapAdapter(container, adapterPrefix, ::Atlas::Message::Element(attributes));
}

MapAdapter* AdapterFactory::createMapAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element)
{
	if (!container) {
		S_LOG_FAILURE("No container sent to AdapterFactory::createMapAdapter.");
		return 0;
	}
	
	if (!element.isMap()) {
		S_LOG_FAILURE("Incorrect element type.");
		return 0;
	}
	
	
	try {
		Window* window = loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/MapAdapter.layout");
		MapAdapter* adapter = new MapAdapter(element, window);
		
		return adapter;
	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("Error when loading MapAdapter. " << ex.getMessage().c_str());
		return 0;
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when loading MapAdapter. " << ex.what());
		return 0;
	}
}

ListAdapter* AdapterFactory::createListAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element)
{
	if (!container) {
		S_LOG_FAILURE("No container sent to AdapterFactory::createListAdapter.");
		return 0;
	}
	
	if (!element.isList()) {
		S_LOG_FAILURE("Incorrect element type.");
		return 0;
	}
	
	try {
		Window* window = loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/ListAdapter.layout");
		ListAdapter* adapter = new ListAdapter(element, window);
		
		return adapter;
	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("Error when loading ListAdapter. " << ex.getMessage().c_str());
		return 0;
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when loading ListAdapter. " << ex.what());
		return 0;
	}
}

StaticAdapter* AdapterFactory::createStaticAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element)
{
	if (!container) {
		S_LOG_FAILURE("No container sent to AdapterFactory::createStaticAdapter.");
		return 0;
	}
	
	if (!(element.isString() || element.isNum())) {
		S_LOG_FAILURE("Incorrect element type.");
		return 0;
	}
	
	try {
		Window* window = loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/StaticAdapter.layout");
		StaticAdapter* adapter = new StaticAdapter(element, window);
		
		return adapter;
	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("Error when loading StaticAdapter. " << ex.getMessage().c_str());
		return 0;
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when loading StaticAdapter. " << ex.what());
		return 0;
	}
}

CEGUI::Window* AdapterFactory::loadLayoutIntoContainer(CEGUI::Window* container, const std::string& adapterPrefix, const std::string& layoutfile)
{
	try {
		CEGUI::WindowManager& windowManager = CEGUI::WindowManager::getSingleton();
	
		std::string finalFileName(GUIManager::getSingleton().getLayoutDir() + layoutfile);
		std::stringstream ss;
		ss << mPrefix << adapterPrefix << (msAutoGenId++);
		mCurrentPrefix = ss.str();
		CEGUI::Window* window = windowManager.loadWindowLayout(finalFileName, mCurrentPrefix);
		container->addChildWindow(window); 
		container->setHeight(window->getHeight());
		container->setWidth(window->getWidth());
		return window;
	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("Error when loading" << layoutfile <<": " << ex.getMessage().c_str());
		return 0;
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Error when loading " << layoutfile <<": " << ex.what());
		return 0;
	}
}


}

}

}

}
