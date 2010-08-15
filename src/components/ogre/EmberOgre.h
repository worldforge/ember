/*
 -----------------------------------------------------------------------------

 Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009

 This program is free software; you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation; either version 2 of the License, or (at your option) any later
 version.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with
 this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 Place - Suite 330, Boston, MA 02111-1307, USA, or go to
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

namespace Eris
{
class View;
class Connection;
class Entity;
}

namespace Carpenter
{
class Carpenter;
class BluePrint;
}

namespace Ember
{
class StreamLogObserver;
class InputCommandMapper;
}

namespace OgreOpcode
{
class CollisionManager;
}

/**
 * @brief Namespace for all classes and functions which interact with Ogre to present the world to the user.
 *
 * In reality that means that this is the single largest namespace in Ember, and as a result of that is further divided into sub-namespaces.
 * @note While a more natural name for this namespace would be Ember::Ogre, such a naming would be to cumbersome as it would clash with the Ogre namespace.
 *
 */
namespace EmberOgre
{

namespace Terrain
{
class TerrainManager;
class TerrainLayerDefinitionManager;
}

class SoundDefinitionManager;

namespace Mapping
{
class EmberEntityMappingManager;
}

namespace Model
{
class ModelDefinitionManager;
class ModelRepresentationManager;
}


namespace Authoring
{
class EntityMoveManager;

class MaterialEditor;

class EntityRecipeManager;

}


class GUIManager;

class Jesus;

class EmberEntity;

class OgreResourceLoader;

class OgreLogObserver;

class OgreSetup;

class OgreResourceProvider;
class OpcodeCollisionDetectorVisualizer;

class ShaderManager;

class ConsoleObjectImpl;

class World;

/**
 @brief The main class of the Ogre rendering component. This functions as a hub for almost all subsystems used when rendering the world.

 This is the central class for the whole Ogre related part of Ember. It's quite large and could perhaps be refactored into smaller parts though.
 It holds references to most subcomponents, and is instantiated at the start of Ember. A lot of different signals from different subsystems are routed through here. If you're not sure how to get access to a certain subsystem from another unrelated class, this is probably the first place you should look.

 It's a singleton so you can access it through
 @code
 EmberOgre::EmberOgre::getSingleton()
 @endcode

 @author Erik Hjortsberg <erik.hjortsberg@gmail.com>

 */
class EmberOgre: public Ember::Singleton<EmberOgre>, public sigc::trackable, public EmberOgreSignals
{
public:

	/**
	 * @brief Ctor.
	 */
	EmberOgre();

	/**
	 * @brief Dtor.
	 */
	~EmberOgre();


	/**
	 * @brief Initialize all Ember services needed for this application
	 *
	 * @param  the prefix for the application, not appliable if running under win32
	 * @param  the an alternative home directory. If the default should be used, send an empty string.
	 */
	void initializeEmberServices(const std::string& prefix, const std::string& homeDir);

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
	 * @brief Returns the main Jesus object, which should be used as the main entry into the Jesus subsystem.
	 *
	 * @return The main Jesus instance.
	 */
	//	Jesus* getJesus() const;

	/**
	 * @brief Gets the main render window.
	 *
	 * The system can contains many different render windows, but there's only one that's the main window.
	 * @return The main render window.
	 */
	Ogre::RenderWindow* getRenderWindow() const;

	/**
	 * @brief Gets the World instance, which represents a scene of a server supplied View instance.
	 *
	 * @returns A World instance, or null if no such exists. The instance is created first when the client has connected and received an Eris::View instance.
	 */
	World* getWorld() const;

	/**
	 * @brief Emitted when the Jesus subsystem has been created.
	 */
	//	sigc::signal<void, Jesus*> EventCreatedJesus;


	/**
	 * @brief Renders one frame.
	 * @return True if rendering occurred.
	 */
	bool renderOneFrame();

	/**
	 * @brief Sets up the application - returns false if the user chooses to abandon configuration.
	 * @return
	 */
	bool setup();

	/**
	 * @brief Shuts down the gui.
	 *
	 * Call this at closing time, right before you destroy the view and shut down the scripting service.
	 */
	void shutdownGui();

	/**
	 * @brief Gets all resource locations.
	 * @returns All resource locations defined. The keys are the resource groups and the values the locations.
	 */
	const std::multimap<std::string, std::string>& getResourceLocations() const;

protected:

	/**
	 * @brief Utility object for setting up and tearing down ogre
	 */
	std::auto_ptr<OgreSetup> mOgreSetup;

	/**
	 * @brief The main Ogre root object. All of Ogre is accessed through this.
	 */
	Ogre::Root *mRoot;

	/**
	 * @brief The main scene manager of the world.
	 */
	Ogre::SceneManager* mSceneMgr;

	/**
	 * @brief The main render window.
	 *
	 * There can be many more render targets in the system, but they will all reside within this render window (such as entity preview through CEGUI).
	 */
	Ogre::RenderWindow* mWindow;

	/**
	 * @brief Utility object to configure shaders
	 */
	ShaderManager* mShaderManager;

	/**
	 * @brief An InputCommandMapper that will handle all general input events.
	 */
	std::auto_ptr<Ember::InputCommandMapper> mGeneralCommandMapper;

	/**
	 * @brief Responsible for handling sound loading
	 */
	SoundDefinitionManager* mSoundManager;

	/**
	 * @brief Responsible for the GUI.
	 */
	GUIManager* mGUIManager;

	/**
	 * @brief Resonsible for managing all Model definitions;
	 */
	Model::ModelDefinitionManager* mModelDefinitionManager;

	/**
	 * @brief Handles all model mappings.
	 */
	Mapping::EmberEntityMappingManager* mEntityMappingManager;

	/**
	 * @brief Responsible for handling all terrain layers.
	 */
	Terrain::TerrainLayerDefinitionManager* mTerrainLayerManager;

	/**
	 * @brief Responsible for handling all entity recipes.
	 */
	Authoring::EntityRecipeManager* mEntityRecipeManager;


	/**
	 * @brief Main entry point for the Jesus system (which is an Ember wrapper for the Carpenter lib)
	 *
	 * @note The Jesus and Carpenter system hasn't been touched in quite a while, and it's not certain that it will remain in Ember.
	 */
	//	Jesus* mJesus;

	/**
	 * @brief The main log observer used for all logging. This will send Ogre logging events on to the internal Ember logging framework.
	 */
	OgreLogObserver* mLogObserver;

	/**
	 * @brief Helper object that allows for easy Ogre material editing.
	 */
	Authoring::MaterialEditor* mMaterialEditor;

	/**
	 * @brief Manages the model representation instances and allows for access of a specific model representation attached to any entity.
	 */
	Model::ModelRepresentationManager* mModelRepresentationManager;

	/**
	 * @brief The scripting resource provider, which loads scripts using the Ogre resource system.
	 */
	std::auto_ptr<OgreResourceProvider> mScriptingResourceProvider;

	/**
	 * @brief The sound resource provider, which loads sounds using the Ogre resource system.
	 */
	std::auto_ptr<OgreResourceProvider> mSoundResourceProvider;

	/**
	 * @brief The collision manager, responsible for handling collisions of the geometry in the world.
	 */
	//	OgreOpcode::CollisionManager* mCollisionManager;

	/**
	 * @brief Responsible for visualizing collisions.
	 */
	//	OpcodeCollisionDetectorVisualizer* mCollisionDetectorVisualizer;

	/**
	 * @brief Handles loading of resources.
	 *
	 * This will also take care of registering our own Ogre::ArchiveFactory instance, so it needs to be destroyed first after ogre is shutdown (since there's no way to remove an already added ArchiveFactory instance from Ogre).
	 */
	OgreResourceLoader* mResourceLoader;

	/**
	 * @brief We hold a reference to our own Ogre log manager, thus making sure that Ogre doesn't create one itself.
	 *
	 * Since we do this we can better steer how Ogre log messages are handled.
	 */
	Ogre::LogManager* mOgreLogManager;

	/**
	 * @brief Set this to true when we're not rendering.
	 *
	 * The reason is that later on, when we resume rendering, we need to reset the event times to prevent various particle effect strangeness.
	 */
	bool mIsInPausedMode;

	/**
	 * @brief The Ogre main camera. This is in most cases handled by mMainCamera.
	 */
	Ogre::Camera* mOgreMainCamera;

	/**
	 * @brief The main console object.
	 */
	ConsoleObjectImpl* mConsoleObjectImpl;

	/**
	 * @brief Gets the main Eris View instance, which is the main inteface to the world.
	 *
	 * The View can also be accessed through the Server service, but this can be used for convenience.
	 * @return The main Eris View instance which represents the server world.
	 */
	Eris::View* getMainView();

	/**
	 * @brief The main World instance will be contained here, when the client is connected to a server and has received a View instance.
	 *
	 * The World instance is only available when the client through an Avatar and View instance has entered into the server side world.
	 */
	World* mWorld;

	/**
	 * @brief Sent from the server service when we've received a Eris::View instance from the server.
	 *
	 * @param world
	 */
	void Server_GotView(Eris::View* world);

	/**
	 * @brief Sent from the server service when the View instance is destroyed.
	 */
	void EntityFactory_BeingDeleted();

	/**
	 * @brief Sets up Jesus. This inialized the mJesus member and loads all building blocks, blueprint and modelblocks etc.
	 */
	//	void setupJesus();


	/**
	 * @brief Preloads the media, thus avoiding frame rate drops ingame.
	 *
	 * Note that preloading media can take quite a while.
	 */
	void preloadMedia();

	/**
	 * @brief Makes sure that there are files in ~/.ember.
	 *
	 * Call this early on at application startup. If the required files cannot be found they are copied there from their default versions as installed in the shared location.
	 */
	void checkForConfigFiles();

	/**
	 * @brief Listen for when all of the application services has been initialized and then register needed objects with them.
	 *
	 * This should occur fairly early on in the application life cycle.
	 */
	void Application_ServicesInitialized();



};

inline Ogre::RenderWindow* EmberOgre::getRenderWindow() const
{
	return mWindow;
}

}

#endif
