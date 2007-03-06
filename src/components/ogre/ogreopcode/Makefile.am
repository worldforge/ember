INCLUDES = -I$(top_srcdir)/src -I$(srcdir)/opcode -I$(srcdir)/include -I$(srcdir)  -I$(top_builddir)/src
METASOURCES = AUTO
noinst_HEADERS =include/OgreOpcode.h include/IOgreCollisionShape.h include/OgreBoxCollisionShape.h include/OgreCollisionContext.h include/OgreCollisionManager.h include/OgreCollisionObject.h include/OgreCollisionReporter.h include/OgreCollisionShape.h include/OgreCollisionTypes.h include/OgreNodes.h include/OgreOpcodeDebugObject.h include/OgreOpcodeExports.h include/OgreOpcodeMath.h include/OgreOpcodeUtils.h include/OgreOrientedBox.h include/OgrePtrCollisionShape.h include/OgreSphereMeshCollisionShape.h include/OgreTriangle.h
SUBDIRS = opcode
noinst_LTLIBRARIES = libOgreOpcode.la
libOgreOpcode_la_LIBADD=opcode/libOpcode.la
libOgreOpcode_la_SOURCES =src/IOgreCollisionShape.cpp src/OgreBoxCollisionShape.cpp src/OgreCollisionContext.cpp src/OgreCollisionManager.cpp src/OgreCollisionShape.cpp src/OgreOpcodeDebugObject.cpp src/OgrePtrCollisionShape.cpp src/OgreSphereMeshCollisionShape.cpp
