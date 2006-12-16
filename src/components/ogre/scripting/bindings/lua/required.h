#include <SDL.h>

#include <sigc++/sigc++.h>

// #include <Eris/View.h>

#include "components/ogre/AvatarController.h"
#include "components/ogre/Avatar.h"
#include "components/ogre/AvatarCamera.h"
#include "components/ogre/EmberOgre.h"

#include "components/ogre/EmberEntityFactory.h"

#include "components/ogre/EmberEntity.h"
#include "components/ogre/EmberPhysicalEntity.h"
// #include "components/ogre/PersonEmberEntity.h"
#include "components/ogre/AvatarEmberEntity.h"
#include "components/ogre/WorldEmberEntity.h"

#include "components/ogre/MousePicker.h"

#include "components/ogre/MotionManager.h"
#include "components/ogre/GUIManager.h"
#include "components/ogre/terrain/TerrainGenerator.h"
#include "components/ogre/terrain/TerrainEditor.h"

#include "components/ogre/model/Model.h"
#include "components/ogre/model/SubModel.h"

#include "components/ogre/widgets/Widget.h"

#include "components/ogre/input/Input.h"

#include "components/ogre/jesus/Jesus.h"

#include "components/ogre/MathConverter.h"

#include "components/ogre/widgets/MovableObjectRenderer.h"
#include "components/ogre/widgets/OgreEntityRenderer.h"
#include "components/ogre/widgets/ModelRenderer.h"
#include "components/ogre/widgets/ListHolder.h"
#include "components/ogre/widgets/Vector3Adapter.h"
#include "components/ogre/widgets/QuaternionAdapter.h"

//#include "components/ogre/widgets/MaterialPicker.h"

#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelDefinition.h"
#include "components/ogre/model/ModelDefinitionManager.h"

#include "components/ogre/IWorldPickListener.h"
#include "components/ogre/EntityWorldPickListener.h"

#include "components/ogre/manipulation/EntityMoveManager.h"

#include "components/ogre/OgreInfo.h"
//#include "components/ogre/scripting/LuaConnector.h"
//#include "LuaConnector.h"

// Ember::EmberServices::getSingletonPtr()->getConfigService()->EventChangedConfigItem.connect(sigc::mem_fun(*this, &AvatarCamera::ConfigService_EventChangedConfigItem));
