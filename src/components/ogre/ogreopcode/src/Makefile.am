INCLUDES = -I$(top_srcdir)/src -I$(srcdir)/include/Opcode -I$(srcdir)/../include -I$(srcdir)  -I$(top_builddir)/src
METASOURCES = AUTO
#noinst_HEADERS = include/OgreOpcode.h include/BP_Endpoint.h include/BP_Proxy.h include/BP_Scene.h include/GEN_List.h include/IOgreCollisionShape.h include/OgreBoxCollisionShape.h include/OgreBroadPhase.h include/OgreCapsule.h include/OgreCollisionContext.h include/OgreCollisionManager.h include/OgreCollisionObject.h include/OgreCollisionReporter.h include/OgreCollisionTypes.h include/OgreEntityCollisionShape.h include/OgreMeshCollisionShape.h include/OgreNodes.h include/OgreOpcodeCharacterController.h include/OgreOpcodeDebugObject.h include/OgreOpcodeLine.h include/OgreOpcodeMath.h include/OgreOpcodeRay.h include/OgreOpcodeTerrainData.h include/OgreOpcodeUtils.h include/OgreOrientedBox.h include/OgrePtrCollisionShape.h include/OgreSphereMeshCollisionShape.h include/OgreTerrainCollisionShape.h include/OgreTriangle.h include/OgreOpcodeExports.h

SUBDIRS = Opcode
noinst_LTLIBRARIES = libOgreOpcode.la
libOgreOpcode_la_LIBADD = \
	$(top_builddir)/src/components/ogre/ogreopcode/src/Opcode/libOpcode.la

libOgreOpcode_la_SOURCES = BP_Endpoint.cpp BP_Proxy.cpp BP_Scene.cpp IOgreCollisionShape.cpp OgreBoxCollisionShape.cpp OgreCapsule.cpp OgreCollisionContext.cpp OgreCollisionManager.cpp OgreCollisionObject.cpp OgreEntityCollisionShape.cpp OgreMeshCollisionShape.cpp OgreOpcodeCharacterController.cpp OgreOpcodeDebugObject.cpp OgreOpcodeLine.cpp OgreOpcodeMath.cpp OgreOpcodeRay.cpp OgreOpcodeTerrainData.cpp OgreOrientedBox.cpp OgrePtrCollisionShape.cpp OgreSphereMeshCollisionShape.cpp OgreTerrainCollisionShape.cpp OgreTriangle.cpp


