/*
 -----------------------------------------------------------------------------

 Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2009

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation; either version 2 of the License, or (at your option) any later
 version.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with
 this program; if not, write to the Free Software Foundation, Inc.,
 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA., or go to
 http://www.gnu.org/copyleft/lesser.txt.
 */

#ifndef __EmberOgre_H__
#define __EmberOgre_H__

#include "EmberOgrePrerequisites.h"

#include "EmberOgreSignals.h"
#include "framework/Singleton.h"

#include <sigc++/trackable.h>

#include <memory>
#include <map>
#include <squall/core/Repository.h>

namespace Eris {
class EventService;

class View;

class Connection;

class Entity;
}

namespace Carpenter {
class Carpenter;

class BluePrint;
}

namespace OgreOpcode {
class CollisionManager;
}

/**
 * @brief Namespace for all classes and functions which interact with Ogre to present the world to the user.
 *
 * In reality that means that this is the single largest namespace in Ember, and as a result of that is further divided into sub-namespaces.
 * @note While a more natural name for this namespace would be Ogre, such a naming would be to cumbersome as it would clash with the Ogre namespace.
 *
 */
namespace Ember {
class InputCommandMapper;

class Input;

class MainLoopController;

class TimeFrame;

class EmberEntity;

class ConfigListenerContainer;

class ServerService;

class SoundService;

namespace Cegui {
class CEGUISetup;
}

namespace OgreView {

namespace Terrain {
class TerrainManager;

class TerrainLayerDefinitionManager;
}

class SoundDefinitionManager;

namespace Lod {
class LodDefinitionManager;

class LodManager;

}

namespace Mapping {
class EmberEntityMappingManager;
}

namespace Model {
class ModelDefinitionManager;
}

namespace Authoring {
class EntityMoveManager;

class MaterialEditor;

class EntityRecipeManager;

}



class ConsoleDevTools;

class GUIManager;

class OgreResourceLoader;

class OgreLogObserver;

class OgreSetup;

class OgreResourceProvider;

class ShaderManager;

class ShaderDetailManager;

class AutomaticGraphicsLevelManager;

class World;

class Screen;

struct OgreLogRouter;

/**
 @brief The main class of the Ogre rendering component. This functions as a hub for almost all subsystems used when rendering the world.

 This is the central class for the whole Ogre related part of Ember. It's quite large and could perhaps be refactored into smaller parts though.
 It holds references to most subcomponents, and is instantiated at the start of Ember. A lot of different signals from different subsystems are routed through here. If you're not sure how to get access to a certain subsystem from another unrelated class, this is probably the first place you should look.

 It's a singleton so you can access it through
 @code
 OgreView::EmberOgre::getSingleton()
 @endcode

 @author Erik Ogenvik <erik@ogenvik.org>

 */
class EmberOgre : public Singleton<EmberOgre>, public virtual sigc::trackable, public EmberOgreSignals {
public:

	/**
	 * @brief Ctor.
	 */
	EmberOgre(MainLoopController& mainLoopController,
			  Eris::EventService& eventService,
			  Input& input,
			  ServerService& serverService,
			  SoundService& soundService,
			  Squall::Repository repository);

	/**
	 * @brief Dtor.
	 */
	~EmberOgre() override;

	/**
	 * @brief Gets the Ogre root object.
	 *
	 * @return The Ogre root object, which acts as an entry point for most of Ogre.
	 */
	Ogre::Root* getOgreRoot() const;

	/**
	 * @brief Gets the shader manager, which handles setup of the graphic level and the shaders.
	 *
	 * @return The shader manager.
	 */
	ShaderManager* getShaderManager() const;

	/**
	 * @brief Gets the automatic graphics level manager, which handles changing of graphics level automatically.
	 * 
	 * @return The automatic graphics level manager.
	 */
	AutomaticGraphicsLevelManager* getAutomaticGraphicsLevelManager() const;

	/**
	 * @brief Gets the main render window.
	 *
	 * The system can contains many different render windows, but there's only one that's the main window.
	 * @return The main render window.
	 */
	Ogre::RenderWindow* getRenderWindow() const;

	/**
	 * @brief Gets the main screen instance.
	 *
	 * @return The main screen instance.
	 */
	Screen& getScreen() const;

	/**
	 * @brief Gets the World instance, which represents a scene of a server supplied View instance.
	 *
	 * @returns A World instance, or null if no such exists. The instance is created first when the client has connected and received an Eris::View instance.
	 */
	World* getWorld() const;

	/**
	 * @brief Renders one frame.
	 * @param timeFrame The time frame within which we can operate while still keeping up the desired frame rate.
	 * @return True if rendering occurred.
	 */
	bool renderOneFrame(const TimeFrame& timeFrame);

	/**
	 * @brief Sets up the application - returns false if the user chooses to abandon configuration.
	 * @param mainLoopController The main loop instance.
	 * @return True if setup was successful.
	 */
	bool setup(MainLoopController& mainLoopController, Eris::EventService& eventService);

	/**
	 * @brief Shuts down the gui.
	 *
	 * Call this at closing time, right before you destroy the view and shut down the scripting service.
	 */
	void shutdownGui();

	void saveConfig();

	std::future<void> loadAssets(Squall::Signature signature);

protected:

	/**
	 * @brief The main input.
	 */
	Input& mInput;

	Ember::ServerService& mServerService;


	std::unique_ptr<OgreLogRouter> mOgreLogRouter;


	/**
	* @brief Handles loading of resources.
	*
	* This will also take care of registering our own Ogre::ArchiveFactory instance,
	 * so it needs to be destroyed first after ogre is shutdown (since there's no way to remove an already added ArchiveFactory instance from Ogre).
	*/
	std::unique_ptr<OgreResourceLoader> mResourceLoader;

	/**
	 * @brief Utility object for setting up and tearing down ogre
	 */
	std::unique_ptr<OgreSetup> mOgreSetup;

	/**
	 * @brief The main Ogre root object. All of Ogre is accessed through this.
	 */
	Ogre::Root* mRoot;

	/**
	 * @brief The Ogre scene manager used when not connected to a world.
	 */
	Ogre::SceneManager* mSceneManagerOutOfWorld;

	/**
	 * @brief The main render window.
	 *
	 * There can be many more render targets in the system, but they will all reside within this render window (such as entity preview through CEGUI).
	 */
	Ogre::RenderWindow* mWindow;

	std::unique_ptr<Cegui::CEGUISetup> mGuiSetup;

	/**
	 * Handles the main rendering window.
	 */
	std::unique_ptr<Screen> mScreen;

	/**
	 * @brief Object that manages level of graphics automatically.
	 */
	std::unique_ptr<AutomaticGraphicsLevelManager> mAutomaticGraphicsLevelManager;

	/**
	 * @brief Utility object to configure shaders
	 */
	std::unique_ptr<ShaderManager> mShaderManager;

	/**
	 * @brief Hooks into the auto adjustment system and alters shaders.
	 */
	std::unique_ptr<ShaderDetailManager> mShaderDetailManager;


	/**
	 * @brief An InputCommandMapper that will handle all general input events.
	 */
	std::unique_ptr<InputCommandMapper> mGeneralCommandMapper;

	/**
	 * @brief Responsible for handling sound loading
	 */
	std::unique_ptr<SoundDefinitionManager> mSoundManager;

	/**
	 * @brief Responsible for the GUI.
	 */
	std::unique_ptr<GUIManager> mGUIManager;

	/**
	 * @brief Resonsible for managing all Model definitions;
	 */
	std::unique_ptr<Model::ModelDefinitionManager> mModelDefinitionManager;

	/**
	 * @brief Handles all model mappings.
	 */
	std::unique_ptr<Mapping::EmberEntityMappingManager> mEntityMappingManager;

	/**
	 * @brief Responsible for handling all terrain layers.
	 */
	std::unique_ptr<Terrain::TerrainLayerDefinitionManager> mTerrainLayerManager;

	/**
	 * @brief Responsible for handling all entity recipes.
	 */
	std::unique_ptr<Authoring::EntityRecipeManager> mEntityRecipeManager;


	/**
	 * @brief Helper object that allows for easy Ogre material editing.
	 */
	std::unique_ptr<Authoring::MaterialEditor> mMaterialEditor;

	/**
	 * @brief The sound resource provider, which loads sounds using the Ogre resource system.
	 */
	std::unique_ptr<OgreResourceProvider> mSoundResourceProvider;

	/**
	 * @brief Manages Lod definition files and loads the Lod information into the mesh.
	 */
	std::unique_ptr<Lod::LodDefinitionManager> mLodDefinitionManager;

	/**
	 * @brief Allows to add Lod to the meshes.
	 */
	std::unique_ptr<Lod::LodManager> mLodManager;

	/**
	 * @brief The collision manager, responsible for handling collisions of the geometry in the world.
	 */
	//	OgreOpcode::CollisionManager* mCollisionManager;
	/**
	 * @brief Responsible for visualizing collisions.
	 */
	//	OpcodeCollisionDetectorVisualizer* mCollisionDetectorVisualizer;

	/**
	 * @brief Set this to true when we're not rendering.
	 *
	 * The reason is that later on, when we resume rendering, we need to reset the event times to prevent various particle effect strangeness.
	 */
	bool mIsInPausedMode;

	/**
	 * @brief The Ogre camera used when not connected to a world.
	 */
	Ogre::Camera* mCameraOutOfWorld;

	/**
	 * @brief The main World instance will be contained here, when the client is connected to a server and has received a View instance.
	 *
	 * The World instance is only available when the client through an Avatar and View instance has entered into the server side world.
	 */
	std::unique_ptr<World> mWorld;

	std::unique_ptr<ConsoleDevTools> mConsoleDevTools;

	std::unique_ptr<ConfigListenerContainer> mConfigListenerContainer;

	/**
	 * @brief Gets the main Eris View instance, which is the main interface to the world.
	 *
	 * The View can also be accessed through the Server service, but this can be used for convenience.
	 * @return The main Eris View instance which represents the server world.
	 */
	Eris::View* getMainView() const;

	/**
	 * @brief Sent from the server service when we've received a Eris::View instance from the server.
	 *
	 * @param world
	 */
	void Server_GotView(Eris::View* world);

	/**
	 * @brief Sent from the server service when the View instance is destroyed.
	 */
	void destroyWorld();

	/**
	 * @brief Preloads the media, thus avoiding frame rate drops ingame.
	 *
	 * Note that preloading media can take quite a while.
	 */
	void preloadMedia();

	/**
	 * @brief Clears the dirty passes lists in Ogre.
	 * @note You need to call this each frame due to problematic behaviour in how Ogre handles multiple scene managers. See http://www.ogre3d.org/mantis/view.php?id=130
	 */
	void clearDirtyPassLists();

	void setupProfiler();

};

inline Ogre::RenderWindow* EmberOgre::getRenderWindow() const {
	return mWindow;
}

}

}

#endif
