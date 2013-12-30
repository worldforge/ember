// To use this precompiled header, you need to inject it with "/FI".

// These headers are at least 4x in *.h files or 10x in *.cpp files

#include <Atlas/Message/Element.h>
#include <Atlas/Objects/Operation.h>

#include <Eris/Connection.h>
#include <Eris/Entity.h>
#include <Eris/TypeInfo.h>
#include <Eris/View.h>

#include <wfmath/axisbox.h>
#include <wfmath/point.h>
#include <wfmath/quaternion.h>
#include <wfmath/vector.h>

#include <OgreCamera.h>
#include <OgreColourValue.h>
#include <OgreCommon.h>
#include <OgreDataStream.h>
#include <OgreEntity.h>
#include <OgreFrameListener.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreMaterial.h>
#include <OgreMaterialManager.h>
#include <OgreMath.h>
#include <OgrePrerequisites.h>
#include <OgreQuaternion.h>
#include <OgreRenderSystem.h>
#include <OgreRenderWindow.h>
#include <OgreResource.h>
#include <OgreResourceManager.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreStringConverter.h>
#include <OgreTexture.h>
#include <OgreTextureManager.h>
#include <OgreVector2.h>
#include <OgreVector3.h>

#include <CEGUI/Event.h>
#include <CEGUI/Window.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/widgets/PushButton.h>

#include <sigc++/bind.h>
#include <sigc++/connection.h>
#include <sigc++/signal.h>
#include <sigc++/slot.h>
#include <sigc++/trackable.h>

#include <cmath>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>