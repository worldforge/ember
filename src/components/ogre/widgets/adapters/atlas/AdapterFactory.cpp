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
		return 0;
	}
	
	CEGUI::Window* window = loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/StringAdapter.layout");
	StringAdapter* adapter = new StringAdapter(element, window);
	
	return adapter;

}

NumberAdapter* AdapterFactory::createNumberAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element)
{
	if (!container) {
		return 0;
	}
	
	CEGUI::Window* window = loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/NumberAdapter.layout");
	NumberAdapter* adapter = new NumberAdapter(element, window);
	
	return adapter;

}

SizeAdapter* AdapterFactory::createSizeAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element)
{
	if (!container) {
		return 0;
	}
	
	try {
		Window* window = loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/SizeAdapter.layout");
		WindowManager& windowMgr = WindowManager::getSingleton();
		Window* lowerX = windowMgr.getWindow(mCurrentPrefix + "lowerX");
		Window* lowerY = windowMgr.getWindow(mCurrentPrefix + "lowerY");
		Window* lowerZ = windowMgr.getWindow(mCurrentPrefix + "lowerZ");
		Window* upperX = windowMgr.getWindow(mCurrentPrefix + "upperX");
		Window* upperY = windowMgr.getWindow(mCurrentPrefix + "upperY");
		Window* upperZ = windowMgr.getWindow(mCurrentPrefix + "upperZ");
		Slider* scaler = static_cast<Slider*>(windowMgr.getWindow(mCurrentPrefix + "scaler"));
		SizeAdapter* adapter = new SizeAdapter(element, lowerX, lowerY, lowerZ, upperX, upperY, upperZ, scaler);
		
		return adapter;
	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("Error when creating SizeAdapter. " << ex.getMessage ().c_str());
		return 0;
	}
}

PositionAdapter* AdapterFactory::createPositionAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element)
{
	if (!container) {
		return 0;
	}
	
	try {
		Window* window = loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/PositionAdapter.layout");
		WindowManager& windowMgr = WindowManager::getSingleton();
		Window* x = windowMgr.getWindow(mCurrentPrefix + "x");
		Window* y = windowMgr.getWindow(mCurrentPrefix + "y");
		Window* z = windowMgr.getWindow(mCurrentPrefix + "z");
		PositionAdapter* adapter = new PositionAdapter(element, x, y, z);
		
		return adapter;
	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("Error when creating PositionAdapter. " << ex.getMessage ().c_str());
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
		return 0;
	}
	Window* window = loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/MapAdapter.layout");
	MapAdapter* adapter = new MapAdapter(element, window);
	
	return adapter;
}

ListAdapter* AdapterFactory::createListAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element)
{
	if (!container) {
		return 0;
	}
	try {
		Window* window = loadLayoutIntoContainer(container, adapterPrefix, "adapters/atlas/ListAdapter.layout");
		ListAdapter* adapter = new ListAdapter(element, window);
		
		return adapter;
	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("Error when loading ListAdapter. " << ex.getMessage().c_str());
		return 0;
	}
}

CEGUI::Window* AdapterFactory::loadLayoutIntoContainer(CEGUI::Window* container, const std::string& adapterPrefix, const std::string& layoutfile)
{
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
}


}

}

}

}
