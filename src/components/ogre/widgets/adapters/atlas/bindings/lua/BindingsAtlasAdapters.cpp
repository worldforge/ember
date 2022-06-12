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

#include "BindingsAtlasAdapters.h"
#include "components/lua/Connector.h"
#include "../../AdapterBase.h"
#include "../../AdapterFactory.h"
#include "../../StaticAdapter.h"
#include "../../AreaAdapter.h"
#include "../../CustomAdapter.h"
#include "../../EntityRefAdapter.h"
#include "../../ListAdapter.h"
#include "../../NumberAdapter.h"
#include "../../StringAdapter.h"
#include "../../SizeAdapter.h"
#include "../../PolygonAdapter.h"
#include "../../PositionAdapter.h"
#include "../../Position2DAdapter.h"
#include "../../MapAdapter.h"
#include "../../ScaleAdapter.h"
#include "../../TerrainModAdapter.h"
#include "../../OrientationAdapter.h"

using namespace Ember::OgreView::Gui::Adapters::Atlas;
using namespace Ember::Lua;

void registerBindingsAtlasAdapters(sol::state_view& lua) {
	auto Ember = lua["Ember"].get_or_create<sol::table>();
	auto OgreView = Ember["OgreView"].get_or_create<sol::table>();
	auto Gui = OgreView["Gui"].get_or_create<sol::table>();
	auto Adapters = Gui["Adapters"].get_or_create<sol::table>();
	auto AdaptersOgre = Adapters["Ogre"].get_or_create<sol::table>();
	auto AdaptersEris = Adapters["Eris"].get_or_create<sol::table>();
	auto AdaptersAtlas = Adapters["Atlas"].get_or_create<sol::table>();


	auto adapterBase = AdaptersAtlas.new_usertype<AdapterBase>("AdapterBase");
	adapterBase["setValue"] = &AdapterBase::setValue;
	adapterBase["getValue"] = &AdapterBase::getValue;
	adapterBase["getOriginalValue"] = &AdapterBase::getOriginalValue;
	adapterBase["updateGui"] = &AdapterBase::updateGui;
	adapterBase["hasChanges"] = &AdapterBase::hasChanges;
	adapterBase["remove"] = &AdapterBase::remove;
	adapterBase["isRemoved"] = &AdapterBase::isRemoved;
	adapterBase["addSuggestion"] = &AdapterBase::addSuggestion;
	adapterBase["EventValueChanged"] = LuaConnector::make_property(&AdapterBase::EventValueChanged);
	auto adapterFactory = AdaptersAtlas.new_usertype<AdapterFactory>("AdapterFactory",
																	 sol::constructors<AdapterFactory(std::string)>());

	adapterFactory["createStringAdapter"] = &AdapterFactory::createStringAdapter;
	adapterFactory["createNumberAdapter"] = &AdapterFactory::createNumberAdapter;
	adapterFactory["createSizeAdapter"] = &AdapterFactory::createSizeAdapter;
	adapterFactory["createScaleAdapter"] = &AdapterFactory::createScaleAdapter;
	adapterFactory["createEntityRefAdapter"] = &AdapterFactory::createEntityRefAdapter;
	adapterFactory["createPositionAdapter"] = &AdapterFactory::createPositionAdapter;
	adapterFactory["createPosition2DAdapter"] = &AdapterFactory::createPosition2DAdapter;
	adapterFactory["createMapAdapter"] = sol::overload(sol::resolve<CEGUI::Window*, const std::string&, const Atlas::Message::Element&>(&AdapterFactory::createMapAdapter),
													   sol::resolve<CEGUI::Window*, const std::string&, const Atlas::Message::MapType&>(&AdapterFactory::createMapAdapter),
													   sol::resolve<CEGUI::Window*, const std::string&, Eris::Entity*>(&AdapterFactory::createMapAdapter));
	adapterFactory["createListAdapter"] = &AdapterFactory::createListAdapter;
	adapterFactory["createOrientationAdapter"] = &AdapterFactory::createOrientationAdapter;
	adapterFactory["createStaticAdapter"] = &AdapterFactory::createStaticAdapter;
	adapterFactory["createAreaAdapter"] = &AdapterFactory::createAreaAdapter;
	adapterFactory["createPolygonAdapter"] = &AdapterFactory::createPolygonAdapter;
	adapterFactory["createTerrainModAdapter"] = &AdapterFactory::createTerrainModAdapter;
	adapterFactory["loadLayoutIntoContainer"] = &AdapterFactory::loadLayoutIntoContainer;
	AdaptersAtlas.new_usertype<AreaAdapter>("AreaAdapter",
											"toggleDisplayOfPolygon", &AreaAdapter::toggleDisplayOfPolygon,
											"addAreaSuggestion", &AreaAdapter::addAreaSuggestion,
											"clearAreaSuggestions", &AreaAdapter::clearAreaSuggestions,
											sol::base_classes, sol::bases<AdapterBase>()
	);

	AdaptersAtlas.new_usertype<EntityRefAdapter>("EntityRefAdapter",
												 sol::base_classes, sol::bases<AdapterBase>()
	);
	AdaptersAtlas.new_usertype<ListAdapter>("ListAdapter",
											"getSize", &ListAdapter::getSize,
											"valueOfAttr", &ListAdapter::valueOfAttr,
											"addAttributeAdapter", &ListAdapter::addAttributeAdapter,
											"removeAdapters", &ListAdapter::removeAdapters,
											sol::base_classes, sol::bases<AdapterBase>()
	);
	AdaptersAtlas.new_usertype<MapAdapter>("MapAdapter",
										   "getAttributeNames", &MapAdapter::getAttributeNames,
										   "valueOfAttr", &MapAdapter::valueOfAttr,
										   "hasAttr", &MapAdapter::hasAttr,
										   "addAttributeAdapter", &MapAdapter::addAttributeAdapter,
										   "removeAdapters", &MapAdapter::removeAdapters,
										   "hasAdapter", &MapAdapter::hasAdapter,
										   sol::base_classes, sol::bases<AdapterBase>()
	);
	AdaptersAtlas.new_usertype<NumberAdapter>("NumberAdapter",
											  sol::base_classes, sol::bases<AdapterBase>()
	);
	AdaptersAtlas.new_usertype<OrientationAdapter>("OrientationAdapter",
												   sol::base_classes, sol::bases<AdapterBase>()
	);
	AdaptersAtlas.new_usertype<PolygonAdapter>("PolygonAdapter",
											   "toggleDisplayOfPolygon", &PolygonAdapter::toggleDisplayOfPolygon,
											   "createNewPolygon", &PolygonAdapter::createNewPolygon,
											   "hasShape", &PolygonAdapter::hasShape,
											   "getShape", &PolygonAdapter::getShape,
											   sol::base_classes, sol::bases<AdapterBase>()
	);
	AdaptersAtlas.new_usertype<Position2DAdapter>("Position2DAdapter",
												  sol::base_classes, sol::bases<AdapterBase>()
	);
	auto positionAdapter = AdaptersAtlas.new_usertype<PositionAdapter>("PositionAdapter",
																	   sol::base_classes, sol::bases<AdapterBase>()
	);
	positionAdapter["EventMoveClicked"] = LuaConnector::make_property(&PositionAdapter::EventMoveClicked);
	AdaptersAtlas.new_usertype<ScaleAdapter>("ScaleAdapter",
											 sol::base_classes, sol::bases<AdapterBase>()
	);
	AdaptersAtlas.new_usertype<SizeAdapter>("SizeAdapter",
											sol::base_classes, sol::bases<AdapterBase>()
	);
	AdaptersAtlas.new_usertype<StaticAdapter>("StaticAdapter",
											  sol::base_classes, sol::bases<AdapterBase>()
	);
	AdaptersAtlas.new_usertype<StringAdapter>("StringAdapter",
											  sol::base_classes, sol::bases<AdapterBase>()
	);
	AdaptersAtlas.new_usertype<TerrainModAdapter>("TerrainModAdapter",
												  "updateGui", &TerrainModAdapter::updateGui,
												  sol::base_classes, sol::bases<AdapterBase>()
	);


}
