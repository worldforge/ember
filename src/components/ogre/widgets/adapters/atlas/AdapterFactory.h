//
// C++ Interface: AdapterFactory
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#ifndef EMBEROGRE_GUI_ADAPTERS_ATLASADAPTERFACTORY_H
#define EMBEROGRE_GUI_ADAPTERS_ATLASADAPTERFACTORY_H
#include <Atlas/Message/Element.h>

namespace CEGUI
{
class Window;
}

namespace Eris
{
class Entity;
}

namespace EmberOgre {

class EmberEntity;

namespace Gui {

namespace Adapters {

namespace Atlas {

class AdapterBase;

class StringAdapter;
class NumberAdapter;
class SizeAdapter;
class MapAdapter;
class ListAdapter;
class PositionAdapter;
class Position2DAdapter;
class OrientationAdapter;
class StaticAdapter;
class AreaAdapter;
class PolygonAdapter;
class TerrainModAdapter;

/**
@brief A factory class through which all adapters are created. Instead of creating the adapters yourself, use this to properly instantiate them.
The factory will take care of loading the gui elements from predefined cegui layout definitions, and setting up all gui elements.
@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class AdapterFactory{
public:
	/**
	 * @brief Ctor.
	 * @param prefix An unique prefix to use when generating the CEGUI windows.
	 */
	AdapterFactory(const std::string prefix);

	/**
	 * @brief Dtor.
	 */
	virtual ~AdapterFactory();

	/**
	* Creates an adapter for a simple string.
	* @param container
	* @param adapterPrefix
	* @param element
	* @return
	*/
	StringAdapter* createStringAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element);

	/**
	* Creates an adapter for a simple number.
	* @param container
	* @param adapterPrefix
	* @param element
	* @return
	*/
	NumberAdapter* createNumberAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element);

	/**
	* Creates an adapter for a sizer widget, which basically is a box where you can alter the size of the box in all three dimensions.
	* @param container
	* @param adapterPrefix
	* @param element
	* @return
	*/
	SizeAdapter* createSizeAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element);

	/**
	 *    Creates an adapter for a 3d position.
	 * @param container
	 * @param adapterPrefix
	 * @param element
	 * @return
	 */
	PositionAdapter* createPositionAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element);
	/**
	 *    Creates an adapter for a 2d position.
	 * @param container
	 * @param adapterPrefix
	 * @param element
	 * @return
	 */
	Position2DAdapter* createPosition2DAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element);
	/**
	 *    Creates an adapter for a map.
	 * @param container
	 * @param adapterPrefix
	 * @param element
	 * @return
	 */
	MapAdapter* createMapAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element);
	/**
	*    Creates an adapter for a map.
	* @param container
	* @param adapterPrefix
	* @param attributes
	* @return
	*/
	MapAdapter* createMapAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::MapType attributes);
	/**
	*    Creates an adapter for a map.
	* @param container
	* @param adapterPrefix
	* @param entity
	* @return
	*/
	MapAdapter* createMapAdapter(CEGUI::Window* container, const std::string& adapterPrefix, Eris::Entity* entity);
	/**
	* Creates an adapter for a quaternion orientation.
	* @param container
	* @param adapterPrefix
	* @param element
	* @return
	*/
	OrientationAdapter* createOrientationAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element);
	/**
	 *    Creates an adapter for a list.
	 * @param container
	 * @param adapterPrefix
	 * @param element
	 * @return
	 */
	ListAdapter* createListAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element);

	/**
	* Creates an adapter for a standard static value, shown in it's simples text form.
	* @param container
	* @param adapterPrefix
	* @param element
	* @return
	*/
	StaticAdapter* createStaticAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element);

	/**
	* @brief Creates an adapter for a terrain area.
	* @param container The container window into which the adapter's windows will be loaded.
	* @param adapterPrefix A unique prefix for the adapter.
	* @param element The element to which the adapter is bound.
	* @param entity The entity to which the terrain area is bound. This can be a null pointer.
	* @return A new terrain area adapter.
	*/
	AreaAdapter* createAreaAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity);

	/**
	* @brief Creates an adapter for a polygon.
	* @param container The container window into which the adapter's windows will be loaded.
	* @param adapterPrefix A unique prefix for the adapter.
	* @param element The element to which the adapter is bound.
	* @param entity The entity to which the terrain area is bound. This can be a null pointer.
	* @return A new polygon adapter.
	*/
	PolygonAdapter* createPolygonAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity);

	/**
	* @brief Creates an adapter for a terrain mod.
	* @param container The container window into which the adapter's windows will be loaded.
	* @param adapterPrefix A unique prefix for the adapter.
	* @param element The element to which the adapter is bound.
	* @param entity The entity to which the terrain area is bound. This can be a null pointer.
	* @return A new polygon adapter.
	*/
	TerrainModAdapter* createTerrainModAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity);

	/**
	 * @brief Creates a new adapter based on a specified type.
	 * @param type The type for which an adapter should be created. Possible values are "string|number|size|position|position2d|map|list|orientation|static".
	 * @param container The container window into which the adapter's windows will be loaded.
	 * @param adapterPrefix A unique prefix for the adapter.
	 * @param element The element to which the adapter is bound.
	 * @param entity An optional entity to which the adapter is connected. This isn't always needed (since an entity instance is not always available)
	 * @return A new adapter instance, or null if there either was an error when creating one, or none could be found for the specified type.
	 */
	AdapterBase* createAdapterByType(std::string type, CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity = 0);

	/**
	 * @brief Loads the gui elements as specified in the submitted layout-file into the container window, using the supplied prefix.
	 * After the layout has been loaded, it will automatically have it's root window added as a child to the submitted container window.
	 * The container window will also have its height and width adjusted to make it fit the newly loaded elements.
	 * @param container The CEGUI window into which the new elements will be loaded.
	 * @param adapterPrefix The unique prefix to use for the new windows.
	 * @param layoutfile The layout file from which to load the elements.
	 * @return The root window of the newly loaded layout. This window has already been added to the supplied container.
	 */
	CEGUI::Window* loadLayoutIntoContainer(CEGUI::Window* container, const std::string& adapterPrefix, const std::string& layoutfile);

	/**
	 * @brief Gets the current prefix used in the latest loaded layout.
	 * @return
	 */
	const std::string& getCurrentPrefix() const;

protected:

	/**
	 * @brief Used for auto generating unique CEGUI window names.
	 */
	static unsigned long msAutoGenId;

	/**
	 * @brief The externally specified prefix used as a base for unique CEGUI window names.
	 */
	std::string mPrefix;

	/**
	 * @brief The prefix used for the current loaded layout.
	 */
	std::string mCurrentPrefix;

	/**
	 * @brief Creates a new adapter for the templated type.
	 * This will in turn call on loadWindowIntoAdapter() for the specific type, but wrap everything in some commonly useful error handling methods.
	 * @param container The container window into which the adapter should be loaded.
	 * @param adapterPrefix The prefix for this unique adapter, used within CEGUI to name the new windows uniquely.
	 * @param element The element for which an adapter should be created.
	 * @param entity An optional entity to which the adapter is connected. This isn't always needed (since an entity instance is not always available)
	 * @return A new adapter instance, or null if there was an error in the setup.
	 */
	template <typename TAdapter> TAdapter* createAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity = 0);

	/**
	 * @brief Verifies that the element is of the correct type for the specific adapter.
	 * @param element The element which should be checked.
	 * @return True if the element was of the correct type for the adapter type.
	 */
	template <typename TAdapter> bool verifyCorrectType(const ::Atlas::Message::Element& element);

	/**
	 * @brief Performs the actual creation of the adapter.
	 * This is called by createAdapter() after some basic checks has been done. This also means that you don't have to do much exception handling in the implementations of this method since that will be taken care of by createAdapter().
	 * @param container The container window into which the adapter should be loaded.
	 * @param adapterPrefix The prefix for this unique adapter, used within CEGUI to name the new windows uniquely.
	 * @param element The element for which an adapter should be created.
	 * @param entity An optional entity to which the adapter is connected. This isn't always needed (since an entity instance is not always available)
	 * @return A new adapter instance, or null if there was an error in the setup.
	 */
	template <typename TAdapter> TAdapter* loadWindowIntoAdapter(CEGUI::Window* container, const std::string& adapterPrefix, const ::Atlas::Message::Element& element, EmberEntity* entity = 0);


};

inline const std::string& AdapterFactory::getCurrentPrefix() const
{
	return mCurrentPrefix;
}

}

}

}

}

#endif
